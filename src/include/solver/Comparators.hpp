#pragma once 

struct CompareUCS {
    template <typename Node>    
    bool operator()(const Node& a, const Node& b) const {
        if (a.value != b.value) return a.value > b.value;
        if (a.state.getMoves().length() != b.state.getMoves().length()) return a.state.getMoves().length() > b.state.getMoves().length();
        return a.state.getNextRequiredNumber() < b.state.getNextRequiredNumber();
    }
};

struct CompareGBFS {
    template <typename Node>
    bool operator()(const Node& a, const Node& b) const {
        if (a.value != b.value) return a.value > b.value;
        if (a.state.getMoves().length() != b.state.getMoves().length()) return a.state.getMoves().length() > b.state.getMoves().length();
        return a.state.getNextRequiredNumber() < b.state.getNextRequiredNumber();
    }
};

struct CompareAStar {
    template <typename Node>
    bool operator()(const Node& a, const Node& b) const {
        if (a.value != b.value) return a.value > b.value;
        if (a.state.getMoves().length() != b.state.getMoves().length()) return a.state.getMoves().length() > b.state.getMoves().length();
        return a.state.getNextRequiredNumber() < b.state.getNextRequiredNumber();
    }
};