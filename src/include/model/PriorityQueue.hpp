#pragma once 

#include <queue>
#include <vector>
#include <cstddef>
#include <stdexcept>

#include "model/State.hpp"

template <typename Compare>
class PriorityQueue {
    private : 
        struct Node {
            State state;
            double value;

            Node(
                const State& state, 
                double value
            ) : 
                state(state),
                value(value) {}
        };

        std::priority_queue<Node, std::vector<Node>, Compare> queue;

    public : 
        PriorityQueue() = default;
        void push(const State& state, double value) {
            queue.push(Node(state, value));
        }

        State pop() {
            if (queue.empty()) throw std::runtime_error("Error : pop in empty queue");

            State temp = queue.top().state;
            queue.pop();

            return temp;
        }

        const State& top() const {
            if (queue.empty()) throw std::runtime_error("Error : top in empty queue");

            return queue.top().state;
        }

        bool empty() const {
            return queue.empty();
        }

        std::size_t size() const {
            return queue.size();
        }

        void clear() {
            while (!queue.empty()) {
                queue.pop();
            }
        }
};  
