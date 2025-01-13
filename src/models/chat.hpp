#pragma once

#include <cstddef>
#include <string>
#include <vector>

class chat {
    using chatId = size_t;
    using userId = size_t;
public:
    void create();

    void addParticipant();

    void removeparticipant();

    void setChatName(const std::string &name);

    bool memberIsParticipant(userId user) const;
public:
    chatId getId() const;

    const std::string &getChatName() const;

    bool chatIsGroup() const;

    const std::vector<userId> &getListParticipants();

private:
    chatId id_;
    std::string chatName_;
    bool isGroup_;
    std::vector<userId> listParticipants_;
};
