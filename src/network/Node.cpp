#include "Block.hpp"
#include "Message.hpp"
#include "Node.hpp"
#include "Peer.hpp"

#include <iostream>

namespace bc
{

void
Node::
start()
{
    running_ = true;
    listener_ = std::thread(&Node::listen, this);
}

void
Node::
stop()
{
    running_ = false;
    if (listener_.joinable())
        listener_.join();
}

void
Node::
add_peer(std::unique_ptr<Peer> peer)
{
    std::lock_guard<std::mutex> lock(peer_mutex_);
    peers_.emplace(peer->id(), std::move(peer));
}

void
Node::
broadcast(const Message& msg)
{
    std::lock_guard<std::mutex> lock(peer_mutex_);
    for (auto& [id, peer] : peers_)
        peer->send(msg.encode());
}

void
Node::
submit(const Transaction& tx)
{
    mempool_.push_back(tx);

    Message msg{ Message_type::NEW_TRANSACTION, tx.serialize() };
    broadcast(msg);
}

void
Node::
mine()
{
    chain_.create_block(mempool_);
    mempool_.clear();

    const Block& b = chain_.get_block(chain_.size() - 1);
    Message msg{ Message_type::NEW_BLOCK, b.serialize() };
    broadcast(msg);
}

bool
Node::
validate(const Block& blk)
{
    if (!blk.validate())
        return false;

    // Check chain continuity
    const Block& tip = chain_.get_block(chain_.size() - 1);
    if (blk.header.prev_hash != tip.hash())
    {
        // Parent not found -> orphan
        orphans_.emplace(blk.header.prev_hash, blk);
        return false;
    }

    return true;
}

void
Node::
listen()
{
    while (running_)
    {
        std::lock_guard<std::mutex> lock(peer_mutex_);

        for (auto& [id, peer] : peers_)
        {
            if (!peer->connected())
                continue;

            std::string raw = peer->receive();
            if (raw.empty())
                continue;

            Message msg = Message::decode(raw);
            handle_message(msg);
        }
    }
}

void
Node::
handle_message(const Message& msg)
{
    if (msg.payload.empty())
        return;

    switch (msg.type)
    {
        case Message_type::NEW_TRANSACTION:
        {
            Transaction tx = Transaction::deserialize(msg.payload);

            if (validate(tx))
            {
                mempool_.push_back(tx);
                broadcast(msg);   // rebroadcast only valid tx
            }
            break;
        }

        case Message_type::NEW_BLOCK:
        {
            Block blk = Block::deserialize(msg.payload);

            if (validate(blk))
            {
                chain_.add_block(blk);
                broadcast(msg);   // rebroadcast only valid block
            }
            break;
        }

        default:
            break;
    }
}

} // namespace bc
