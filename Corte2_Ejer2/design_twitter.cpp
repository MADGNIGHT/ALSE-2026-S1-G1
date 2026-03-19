#include <iostream>
#include <vector>
using namespace std;

class Twitter
{
public:
    vector<vector<int>> following; // Lista de seguimiento para cada usuario (userId -> lista de followees)
    vector<pair<int, int>> tweets; // (userId, tweetId)

    Twitter()
    {
        following.resize(501); // Tamaño de usuarios
    }

    void postTweet(int userId, int tweetId) // Recibir un userId y un tweetId, y almacenar el tweet para ese usuario
    {
        tweets.push_back({userId, tweetId}); // Agregar el tweet a la lista de tweets con su userId correspondiente
    }

    vector<int> getNewsFeed(int userId) // Recibir un user Id y presetnar los 10 tweetIds
    {
        vector<int> feed;
        for (int i = tweets.size() - 1; i >= 0 && feed.size() < 10; i--) // Recorrer la lista de tweets
        {
            int user = tweets[i].first;
            if (user == userId) // si es el mismo usuario, agregar el tweet al feed
            {
                feed.push_back(tweets[i].second);
            }
            else // si no es el mismo usuario, verificar si el usuario del tweet es seguido por el userId dado
            {
                for (int followee : following[userId])
                {
                    if (followee == user)
                    {
                        feed.push_back(tweets[i].second);
                        break;
                    }
                }
            }
        }
    }

    void follow(int followerId, int followeeId) // Recibir un followerId y un followeeId, y hacer que el followerId siga al followeeId
    {
        if (followerId == followeeId)
            return; // No puede seguirse a sí mismo

        for (int followee : following[followerId])
        {
            if (followee == followeeId)
                return; // Ya sigue a este usuario
        }
        following[followerId].push_back(followeeId);
    }

    void unfollow(int followerId, int followeeId) // Recibir un followerId y un followeeId, y hacer que el followerId deje de seguir al followeeId
    {
        for (auto it = following[followerId].begin(); it != following[followerId].end(); ++it) // Pasar por la lista de followees
        {
            if (*it == followeeId) // Si se encuentra el followeeId en la lista de seguimiento del followerId, eliminarlo
            {
                following[followerId].erase(it);
                break;
            }
        }
    }
};
