/**
 * @file auction_system.cpp
 * @brief Sistema de subastas.
 */

#include <unordered_map>
using namespace std;

/**
 * @class AuctionSystem
 * @brief Maneja ofertas de usuarios sobre items.
 */
class AuctionSystem
{
private:
    unordered_map<int, unordered_map<int, int>> bids; ///< itemId (userId para ver monto bidAmount)
public:
    AuctionSystem() {}

    /** @brief Añade una oferta */
    void addBid(int userId, int itemId, int bidAmount)
    {
        bids[itemId][userId] = bidAmount;
    }

    /** @brief Actualiza una oferta */
    void updateBid(int userId, int itemId, int newAmount)
    {
        bids[itemId][userId] = newAmount;
    }

    /** @brief Elimina una oferta */
    void removeBid(int userId, int itemId)
    {
        bids[itemId].erase(userId);
    }

    /**
     * @brief Obtiene el mejor postor.
     * @return userId si hacen oferta o si no -1 si no hay ofertas.
     */
    int getHighestBidder(int itemId)
    {
        if (!bids.count(itemId) || bids[itemId].empty())
            return -1;

        int bestUser = -1, bestBid = -1;

        for (auto &p : bids[itemId])
        {
            int user = p.first, bid = p.second;
            if (bid > bestBid || (bid == bestBid && user > bestUser))
            {
                bestBid = bid;
                bestUser = user;
            }
        }
        return bestUser;
    }
};
