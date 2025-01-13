#pragma once

#include <cstddef>
#include <string>
#include <optional>

class message {
    using messageId = size_t;
    using userId = size_t;
    using chatId = size_t;
public:
    static message create();
public:
    messageId getId() const;

    chatId getChatId() const;

    userId getSenderId() const;

    std::optional<userId> getRecipientId() const;

    const std::string &getContent() const;

    time_t getTimestamp() const;

    std::optional<time_t> getReceivedDate() const;

    bool isRead() const;

    void markAsReceived();

    void markAsRead();

private:
    messageId id_;
    chatId chatId_;
    userId from_;
    std::optional<userId> to_;
    std::string body_;
    time_t sentDate_;
    std::optional<time_t> receivedDate_;
    bool isRead_;
};