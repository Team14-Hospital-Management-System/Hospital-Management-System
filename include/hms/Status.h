#pragma once
// Shared success/failure types. FROZEN: change only via team-approved PR.
#include <string>
#include <utility>

namespace hms {

struct Status {
    bool ok = true;
    std::string message;

    static Status success() { return Status{true, {}}; }
    static Status failure(std::string msg) { return Status{false, std::move(msg)}; }
    explicit operator bool() const { return ok; }
};

// A value plus a Status. If !ok(), `value` is default-constructed and must be ignored.
template <typename T>
struct Result {
    Status status;
    T value{};

    bool ok() const { return status.ok; }
    static Result success(T v) {
        Result r;
        r.status = Status::success();
        r.value = std::move(v);
        return r;
    }
    static Result failure(std::string msg) {
        Result r;
        r.status = Status::failure(std::move(msg));
        return r;
    }
};

}  // namespace hms
