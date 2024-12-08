local function sf()
    for i = 1, math.floor(sv.getSize() / 2), 1 do
        sv.swap(i, sv.getSize() - i + 1)
    end
end

return {
    shuffle = sf,
    tasks = {
        {
            algorithmName = "Bubble sort",
            arraySize = 256
        },
        {
            algorithmName = "Optimized bubble sort",
            arraySize = 256
        },
        {
            algorithmName = "Cocktail shaker sort",
            arraySize = 256
        },
        {
            algorithmName = "Optimized cocktail shaker sort",
            arraySize = 256
        },
        {
            algorithmName = "Odd even sort",
            arraySize = 256
        },
        {
            algorithmName = "Comb sort",
            arraySize = 1024
        },
        {
            algorithmName = "Hybrid comb sort",
            arraySize = 1024
        },
        {
            algorithmName = "Circle sort",
            arraySize = 2048
        },
        {
            algorithmName = "Selection sort",
            arraySize = 256
        },
        {
            algorithmName = "Double selection sort",
            arraySize = 256
        },
        {
            algorithmName = "Exchange sort",
            arraySize = 256
        },
        {
            algorithmName = "Cycle sort",
            arraySize = 256
        },
        {
            algorithmName = "Heap sort",
            arraySize = 2048
        },
        {
            algorithmName = "Min heap sort",
            arraySize = 2048
        },
        {
            algorithmName = "Weak heap sort",
            arraySize = 2048
        },
        {
            algorithmName = "Smooth sort",
            arraySize = 2048
        },
        {
            algorithmName = "Poplar sort",
            arraySize = 2048
        },
        {
            algorithmName = "Insertion sort",
            arraySize = 256
        },
        {
            algorithmName = "Binary insertion sort",
            arraySize = 256
        },
        {
            algorithmName = "Shell sort",
            arraySize = 1024
        },
        {
            algorithmName = "Quick sort (left/right pointers)",
            arraySize = 256
        },
        {
            algorithmName = "Quick sort (left/left pointers)",
            arraySize = 256
        },
        {
            algorithmName = "Dual pivot quick sort",
            arraySize = 256
        },
        {
            algorithmName = "Proportion extend sort",
            arraySize = 1024
        },
        {
            algorithmName = "Intro sort",
            arraySize = 2048
        },
        {
            algorithmName = "Pattern defeating quick sort",
            arraySize = 2048
        },
        {
            algorithmName = "Merge sort",
            arraySize = 2048
        },
        {
            algorithmName = "Iterative merge sort",
            arraySize = 2048
        },
        {
            algorithmName = "In place merge sort",
            arraySize = 1024
        },
        {
            algorithmName = "Weave sort",
            arraySize = 1024
        },
        {
            algorithmName = "Rotate merge sort",
            arraySize = 2048
        },
        {
            algorithmName = "Tim sort",
            arraySize = 2048
        },
        {
            algorithmName = "Bitonic sort",
            arraySize = 2048
        },
        {
            algorithmName = "Odd even merge sort",
            arraySize = 2048
        },
        {
            algorithmName = "Pairwise sorting network",
            arraySize = 2048
        },
        {
            algorithmName = "LSD radix sort",
            arraySize = 4069
        },
        {
            algorithmName = "MSD radix sort",
            arraySize = 4096
        },
        {
            algorithmName = "Bucket sort",
            arraySize = 4069
        },
        {
            algorithmName = "Binary quick sort",
            arraySize = 2048
        },
        {
            algorithmName = "Pancake sort",
            arraySize = 256
        },
        {
            algorithmName = "Stooge sort",
            arraySize = 256
        },
        {
            algorithmName = "Slow sort",
            arraySize = 128
        }
    }
}
