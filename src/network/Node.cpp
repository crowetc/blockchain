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
    chain_.add_block(mempool_);
    mempool_.clear();

    const Block& b = chain_.get_block(chain_.size() - 1);
    Message msg{ Message_type::NEW_BLOCK, b.serialize() };
    broadcast(msg);
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

    if (msg.type == Message_type::NEW_TRANSACTION)
    {
        Transaction tx = Transaction::deserialize(msg.payload);
        mempool_.push_back(tx);
    }
    else if (msg.type == Message_type::NEW_BLOCK)
    {
        Block b = Block::deserialize(msg.payload);
        chain_.add_block(b.body.transactions);
    }
}

} // namespace bc
