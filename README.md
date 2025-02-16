# Custom STL Containers

This project is an implementation of several standard STL containers from scratch, along with a custom allocator and utility functions such as `std::forward<T>` and `std::move`.

## Implemented Containers

- `vector`
- `list`, `forward_list`
- `deque`
- `trees`
  - Binary Search Tree (BST)
  - Iterators for the search tree
  - N-ary tree

## Additional Features

- **Allocator**: A simple custom allocator for memory management.
- **Utility Functions**:
  - Implementation of `std::move`
  - Implementation of `std::forward<T>`

## Project Features

- **Fully-fledged STL container implementations** supporting move semantics, pointers, and memory management.
- **Algorithms**, including search tree threading with iterators.
- **Configured CI**: Automatic pull request validation via GitHub Actions.
- **Unit Testing**: All containers and utility functions are tested using Google Test (`gtest`).

## Installation and Running Tests

```sh
# Clone the repository
git clone https://github.com/username/repository.git
cd repository

# Build and run tests
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

## Requirements
- A compiler supporting C++17 or later
- CMake (for building)
- Google Test (automatically installed during the build)

