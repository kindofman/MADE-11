/*
13. «Пятнашки»
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
[ 1  2  3  4 ]
[ 5  6  7  8 ]
[ 9  10 11 12] 
[ 13 14 15 0 ]
где 0 задает пустую ячейку.
Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
Формат входных данных
Начальная расстановка.
Формат выходных данных
Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек, 
которое нужно сделать, чтобы достичь выигрышной конфигурации, а во второй строке выведите соответствующую последовательность
ходов: L означает, что костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если таких последовательностей несколько, 
то выведите любую из них. Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.
*/

#include <iostream>
#include <algorithm>
#include <array>
#include <queue>
#include <unordered_map>
#include <vector>
#include <assert.h>


struct position {
    std::vector<char> chips;
    char zero_place;

    bool IsFinish() const;
    std::vector<position> Siblings() const;

    bool operator==(const position& other) const;
    bool operator!=(const position& other) const { return !operator==(other); }

    int h() {
        int result = 0;
        int x1, y1, x2, y2;
        for ( int i = 0; i < chips.size(); ++i ) {
        	int num = chips[i];
        	if ( num == 0 ) { continue; }
            x1 = i % 4;
            y1 = i / 4;
            x2 = (num - 1) % 4;
            y2 = (num - 1) / 4;
            result += abs(x2 - x1) + abs(y2 - y1);
        }
        return result;
    };
};

struct Less {
    const bool operator()( position& left, position& right ) const {
        return left.h() > right.h();
    };
};

bool position::operator==(const position& other) const {
    for (size_t i = 0; i < chips.size(); ++i ) {
        if (chips[i] != other.chips[i]) return false;
    }
    return true;
}
const position FinishPosition{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}, 15};


bool position::IsFinish() const {
  return operator==(FinishPosition);
}

std::vector<position> position::Siblings() const {
    std::vector<position> result;
    if (zero_place < 12) {
        position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 4]);
        sibling.zero_place += 4;
        result.emplace_back(sibling);
  }
    if (zero_place >= 4) {
        position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 4]);
        sibling.zero_place -= 4;
        result.emplace_back(sibling);
  }
    if (zero_place % 4 != 0) {
        position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 1]);
        sibling.zero_place -= 1;
        result.emplace_back(sibling);
  }
    if (zero_place % 4 != 3) {
        position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 1]);
        sibling.zero_place += 1;
        result.emplace_back(sibling);
  }
    return result;
}

const size_t HashParameter = 7;
namespace std {
    template <>
    struct hash<position> {
        size_t operator()(const position& key) const {
            size_t value = 0;
            for (auto chip : key.chips) {
                value = HashParameter * value + static_cast<size_t>(chip);
            }
            return value;
        }
    };
}

bool BFS(const position& start, std::unordered_map<position, position>& parents) {
    std::priority_queue<position, std::vector<position>, Less> positions_queue;
    positions_queue.push(start);
    while (!positions_queue.empty()) {
        position current = positions_queue.top();
        positions_queue.pop();
        for (auto& sibling : current.Siblings()) {
            if (parents.count(sibling)) {
                continue;
            }
            parents[sibling] = current;
            if (sibling.IsFinish()) {
                return true;
            }
            positions_queue.push(sibling);
        }
    }
    return false;
}


char GetMoveSymbol(const position& from, const position& to) {
    char zero_diff = to.zero_place - from.zero_place;
    switch (zero_diff) {
        case 1:
            return 'L'; // Ноль вправо -> фишка влево
        case -1:
            return 'R';
        case 4:
            return 'U';
        case -4:
            return 'D';
        default:
            assert(false);
    }
    return 0;
}

std::vector<char> GetPath(const position& start, const std::unordered_map<position, position>& parents) {
    std::vector<char> result;

    position current = FinishPosition;
    while (current != start) {
        const position parent = parents.at(current);
        result.push_back(GetMoveSymbol(parent, current));
        current = parent;
    }
  // Развернем result, так как собирали его с конца.
    std::reverse(result.begin(), result.end());
    return result;
}

std::pair<bool, std::vector<char>> SolvePuzzle15(const position& start) {
  // Запустим BFS, запоминая предков всех пройденных позиций.
    std::unordered_map<position, position> parents;
    if (!BFS(start, parents)) {
        return std::make_pair(false, std::vector<char>());
    }
    return std::make_pair(true, GetPath(start, parents));
}


template <typename T>
using Comparator = bool (*)(const T&, const T&);


//склеивает массивы и считает инверсии
template <typename T>
int64_t mergeAndCount(T* inputArray1, size_t lenFirst,  T* inputArray2,
            size_t lenSecond, T* result, int64_t currentCounter, Comparator<T> Tless) {
    int64_t newCounter(currentCounter);
    size_t ii(0), jj(0);
    for(size_t i = 0; i < lenFirst + lenSecond; ++i) {
        if(ii < lenFirst and jj < lenSecond) {
            if(Tless(inputArray2[jj], inputArray1[ii])) {
                result[i] = inputArray2[jj];
                newCounter = newCounter + lenFirst - ii;
                jj += 1;
            } else {
                result[i] = inputArray1[ii];
                ii += 1;
            }
        } else if (jj < lenSecond) {
            result[i] = inputArray2[jj];
            jj += 1;
        } else {
            result[i] = inputArray1[ii];
            ii += 1;
        }
    }
    return newCounter;
}

//сортирует и считает инверсии с помощью sortAndCount
template <typename T>
int64_t sortAndCount(T* inputArray, size_t len, int64_t currentCounter, Comparator<T> Tless) {
    if(len <= 1) {
        return currentCounter;
    } else {
        int64_t newCounter(currentCounter);
        size_t lenFirst  = len / 2;
        size_t lenSecond = len - lenFirst;

        T* bufferArray = new T[len];

        newCounter = sortAndCount(inputArray, lenFirst, newCounter, Tless);
        newCounter = sortAndCount(inputArray+lenFirst, lenSecond, newCounter, Tless);

        newCounter = mergeAndCount(inputArray, lenFirst, inputArray+lenFirst,
                                            lenSecond, bufferArray, newCounter, Tless);
        std::copy(bufferArray, bufferArray + len, inputArray);

        delete [] bufferArray;
        return newCounter;
    }
}

template <typename T>
bool compareFunc(const T& T1, const T& T2) {
    return (T1 < T2);
}

bool hasSolution8(position& start) {
    std::vector<char> arrayToSort;
    for (auto chip : start.chips) {
        if(chip)
            arrayToSort.push_back(chip);
    }

    int invertion(0);
    invertion = sortAndCount(&arrayToSort[0], arrayToSort.size(), 0, compareFunc);
    return !(bool)( invertion % 2);
}

bool hasSolution15(position& start) {
    std::vector<char> arrayToSort;
    for (auto chip : start.chips) {
        if(chip)
            arrayToSort.push_back(chip);
    }

    int invertion(0);
    invertion = sortAndCount(&arrayToSort[0], arrayToSort.size(), 0, compareFunc);
    int pos = (int)start.zero_place;
  
    pos = (int) pos / 4 + 1;

    invertion += pos;
    return !(bool)( invertion % 2);
}

int main() {
  position start;

  int buffer;
  for(int i = 0; i < 16; ++i) {
      std::cin >> buffer;
      if(buffer == 0)
          start.zero_place = i;

      buffer = buffer;
      start.chips.push_back(buffer);
  }

    if (hasSolution15(start)) {
        const auto result = SolvePuzzle15(start);

        std::cout << result.second.size() << std::endl;

        for (const auto move : result.second)
            std::cout << move;

        std::cout << std::endl;
    } else {
        std::cout << -1 << std::endl;
    }
    return 0;
}

