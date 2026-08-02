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
    Node(uint16_t port, uint32_t difficulty);

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

    /** Submit a transaction to the mempool and broadcast it to peers. */
    void
    submit(const Transaction& tx);

    /** Mine a new block from the mempool and broadcast it. */
    void
    mine();

private:
    // Networking
    uint16_t port_;
    std::unordered_map<std::string, std::unique_ptr<Peer>> peers_;
    std::mutex peer_mutex_;
    std::thread listener_;

    // Blockchain
    Chain chain_;
    std::vector<Transaction> mempool_;

    std::atomic<bool> running_;

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
Node(uint16_t port, uint32_t difficulty)
: port_(port),
  chain_({}, difficulty),
  running_(false)
{}

} // namespace bc

#endif // NODE_HPP_
