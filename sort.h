#ifndef SORT_H
#define SORT_H


namespace my {
template <typename Iterator, typename Comparator>
void sort(Iterator begin, Iterator end, Comparator comp)
{
    for (auto i = begin; i != end; ++i) {
        auto min_it = i;
        for (auto j = i + 1; j != end; ++j) {
            if (comp(*j, *min_it)) {
                min_it = j;
            }
        }
        if (min_it != i) {
            auto tmp = *i;
            *i = *min_it;
            *min_it = tmp;
        }
    }
}

template <typename Iterator>
void sort(Iterator begin, Iterator end)
{
    my::sort(begin, end, [](const auto& a, const auto& b) {
        return a < b;
    });
}

}

#endif // SORT_H