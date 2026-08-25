#ifndef NODE_HPP_
#define NODE_HPP_

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>

#include "Chain.hpp"
#include "Transaction.hpp"

namespace bc
{

// Forward Declarations
class Message;
class Peer;

/** Node
 *
 *  Represents a single participant in the blokchain network.
 *
 *  Responsibilities:
 *  - Maintain a local blockchain
 *  - Maintain a mempool of pending transactions
 *  - Connect to remote peers and exchange messages
 *  - Broadcast new transactions and mine blocks
 *  - Listen for incoming messages
 *
 *  Concurrency Model:
 *  - The Node runs a dedicated listener thread that polls all connected peers
 *  - The main thread may submit transactions, mine blocks, or initiate peer connections
 *  - Access to the peer map is synchronized with a mutex
 */
class Node
{
public:
    /** Construct a Node
     *  @param port The local port this node will listen on
     *  @param difficulty Mining difficulty
     */
    Node(std::uint16_t port, std::uint32_t difficulty);

    /** Start the node.
     *
     *  Launches the listener thread and begins processing incoming messages.
     */
    void
    start();

    /** Stop the node.
     *
     *  Signals the listener thread to exit and joins it.
     */
    void
    stop();

    /** Add a peer to the node.
     *
     *  Transfers ownership of a connected Peer into the node’s peer map.
     */
    void
    add_peer(std::unique_ptr<Peer> peer);

    /** Broadcast a message to all connected peers. */
    void
    broadcast(const Message& msg);

    /** Process a raw inbound message from the network.
     *
     *  Decodes the message and dispatches it through the node's internal consensus logic.
     */
    void
    receive_message(const std::string& raw);

    /** Submit a transaction to the mempool and broadcast it to peers. */
    void
    submit(const Transaction& tx);

    /** Mine a new block from the mempool and broadcast it. */
    void
    mine();

private:
    // Networking
    std::uint16_t port_;
    std::unordered_map<std::string, std::unique_ptr<Peer>> peers_;
    std::mutex peer_mutex_;
    std::thread listener_;

    // Blockchain
    Chain chain_;
    std::vector<Transaction> mempool_;
    std::unordered_map<std::string, Block> orphans_;

    std::atomic<bool> running_;

    /** Validate a transaction at the node level.
     *
     *  Performs basic structural checks and node‑specific policy checks.
     */
    bool
    validate(const Transaction& tx);

    /** Validate a block at the node level.
     *
     *  Ensures the block is valid and correctly extends the local chain tip.
     */
    bool
    validate(const Block& blk);

    /** Listener loop executed in the background thread.
     *
     *  Continuously polls each connected peer for incoming messages.
     *  Decodes messages and dispatches them to handle_message().
     */
    void
    listen();

    /** Handle an incoming message.
     *
     *  Dispatches based on message type:
     *  - NEW_TRANSACTION: add to mempool
     *  - NEW_BLOCK: append block to chain
     */
    void
    handle_message(const Message& msg);
};

//
// Inline Implementations
//

inline
Node::
Node(std::uint16_t port, std::uint32_t difficulty)
: port_(port),
  chain_({}, difficulty),
  running_(false)
{}

inline
bool
Node::
validate(const Transaction& tx)
{
    return tx.validate();
}

} // namespace bc

#endif // NODE_HPP_
