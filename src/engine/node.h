#pragma once

#include "state.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class Node {
public:
    virtual void render() = 0;

    virtual ~Node() { onDestroy(); }

    Node(const Node&) = default;
    Node(Node&&) = default;
    Node& operator=(const Node&) = default;
    Node& operator=(Node&&) = default;

protected:
    std::shared_ptr<TState> state;

    Node(const std::shared_ptr<TState>& state) : state(state) { onCreate(); }

    virtual void onCreate() {};

    virtual void onDestroy() {};
};

}
