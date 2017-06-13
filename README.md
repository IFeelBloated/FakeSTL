# nSTL
@2017 IFeelBloated, nSTL (naive Standard Template Library)

## License
LGPL v3.0

## Description
nSTL is my attempt of doing some library stuff, it's **NOT** meant to replace the existing STL implementations, it's simply something that's gonna help with your data structure and algorithm analysis courses, it demonstrates how you could put all that mathematical concepts to bed, like wrap them up to a library like STL.

the main problem with the existing STL implementations for this purpose is the readability issue, the source code of the existing implementations looks pretty much like machine code (to me at least), and nSTL is the kind of implementation that tries to maximize readability, you could probably understand the nSTL source code like even without any comments.

## Requirements
- For U: know the basics about data structures, algorithms and modern C++
- For the compiler/IDE: at least support ISO/IEC 14882:2014(C++14) and "alloca" (dynamic memory allocation on stack)

## Containers/Structures
- *StandardTemplateLibrary::List*: an incomplete implementation of std::list, circular doubly linked list
- *StandardTemplateLibrary::String/WString/U16String/U32String*: an incomplete implementation of std::string/wstring/u16string/u32string, dynamic array of characters
- *StandardTemplateLibrary::Stack*: an incomplete implementation of std::stack, stack adapter

## Algorithms
- *[Merge Sort](https://en.wikipedia.org/wiki/Merge_sort)*: StandardTemplateLibrary::List::Sort()
- *[Knuth–Morris–Pratt Algorithm](https://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm)*: StandardTemplateLibrary::BasicString::Find(...)
- *[Introsort](https://en.wikipedia.org/wiki/Introsort) ([Quicksort](https://en.wikipedia.org/wiki/Quicksort) + [Heapsort](https://en.wikipedia.org/wiki/Heapsort))*: StandardTemplateLibrary::Sort(...)

## Demonstrations
- *Polynomial Multiplication*: Demo/Polynomial-List.cpp
- *Expression Evaluation*: Demo/Expression Evaluation-Stack.cpp

## Extras
- *[Sparse Matrix Multiplication](https://en.wikipedia.org/wiki/Sparse_matrix#List_of_lists_.28LIL.29)*: StandardTemplateLibrary::Extras::SparseMatrix::operator*=(...)
- *[Prim's Algorithm (Minimum Spanning Tree)](https://en.wikipedia.org/wiki/Prim%27s_algorithm)*: StandardTemplateLibrary::Extras::Graph::GenerateMinimumSpanningTree(...)
- *[Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding)*: StandardTemplateLibrary::Extras::HuffmanTree::HuffmanTree(...) / TagEachNodeRecursively(...)