#include <iostream>
#include <vector>
#include <algorithm>

void bubbleSort(std::vector<int> &a)
{
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n; i++)
    {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (a[j] > a[j + 1])
            {
                swapped = true;
                std::swap(a[j], a[j + 1]);
            }
        }
        if (!swapped)
        {
            break;
        }
    }
}

void selectionSort(std::vector<int> &a)
{
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n; ++i)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (a[minIndex] > a[j])
            {
                minIndex = j;
            }
        }
        std::swap(a[minIndex], a[i]);
    }
}

void insertionSort(std::vector<int> &a)
{
    int n = static_cast<int>(a.size());
    for (int i = 1; i < n; ++i)
    {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void merge(std::vector<int> &a, int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;
    std::vector<int> tmp;
    while (i <= mid && j <= right)
    {
        if (a[i] <= a[j])
        {
            tmp.push_back(a[i]);
            ++i;
        }
        else
        {
            tmp.push_back(a[j]);
            ++j;
        }
    }
    while (i <= mid)
    {
        tmp.push_back(a[i]);
        ++i;
    }
    while (j <= right)
    {
        tmp.push_back(a[j]);
        ++j;
    }

    for (int k = 0; k < tmp.size(); ++k)
    {
        a[left + k] = tmp[k];
    }
}

void mergeSort(std::vector<int> &a, int left, int right)
{
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);
    merge(a, left, mid, right);
}

int partionArray(std::vector<int> &a, int left, int right)
{
    int pivot = a[right];
    int i = left - 1;
    for (int j = left; j < right; ++j)
    {
        if (a[j] <= pivot)
        {
            i++;
            std::swap(a[i], a[j]);
        }
    }
    std::swap(a[i + 1], a[right]);
    return i + 1;
}

void quickSort(std::vector<int> &a, int left, int right)
{
    if (left >= right)
        return;
    int pivot = partionArray(a, left, right);
    quickSort(a, left, pivot - 1);
    quickSort(a, pivot + 1, right);
}

int main()
{
    std::vector<int> a = {5, 2, 9, 1, 3};
    int left = 0, right = static_cast<int>(a.size() - 1);
    quickSort(a, left, right);

    for (int x : a)
    {
        std::cout << x << " ";
    }

    return 0;
}