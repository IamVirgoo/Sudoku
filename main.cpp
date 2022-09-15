#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

typedef vector<vector<int>> Board;
typedef set<int> Values;

class Sudoku {
    public:
        /**
         * @name <h1>getPosition</h1>
         *
         * @param row_index - индекс элемента строки
         * @param column_index - индекс элемента колонки
         * @param board - передоваемое поле судоку
         *
         * @description Получение значения из поля типа Board
         * @return int Type of board[0][0]
         **/
        int getPosition(int row_index, int column_index, Board& board) {
            return board[row_index][column_index];
        }
        /**
         * @name <h1>setPosition</h1>
         *
         * @param row_index - индекс элемента строки
         * @param column_index - индекс элемента колонки
         * @param element - значение setter'а
         * @param board - передоваемое поле судоку
         *
         * @description Установление значения element в ячейку поля Board
         * @return void
         * */
        void setPosition(int row_index, int column_index, int element, Board& board) {
            swap(board[row_index][column_index], element);
        }
        /**
         * @name <h1>getRowValues</h1>
         *
         * @param row_index - индекс элемента строки
         * @param board - передоваемое поле судоку
         *
         * @description Создание множества values и заполнение его значениями из строки поля Board
         * @return Values or set<int>
         * */
        Values getRowValues(int row_index, Board& board) {
            Values values;
            for (auto el : board[row_index]) {
                values.insert(el);
            }
            return values;
        }
        /**
         * @name <h1>getColumnValues</h1>
         *
         * @param column_index - индекс элемента колонки
         * @param board - передоваемое поле судоку
         *
         * @description Создание множества values и заполнение его значениями из колонки поля Board
         * @return Values or set<int>
         * */
        Values getColumnValues(int column_index, Board& board) {
            Values values;
            for (int el = 0; el < 9; el++) {
                values.insert(board[el][column_index]);
            }
            return values;
        }
        /**
         * @name <h1>getGroupValues</h1>
         *
         * @param row_index - индекс элемента строки
         * @param column_index - индекс элемента колонки
         * @param board - передоваемое поле судоку
         *
         * @description Создание и заполнение множества values из значений одного блока 3 на 3 из поля Board
         * @return Values or set<int>
         * */
        Values getGroupValues(int row_index, int column_index, Board& board) {
            Values values;
            int groupRowStart = 3 * (row_index / 3);
            int groupColumnStart = 3 * (column_index / 3);
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    values.insert(board[groupRowStart + i][groupColumnStart + j]);
                }
            }
            return values;
        }
        /**
         * @name <h1>findPossibleValues</h1>
         *
         * @param board - передоваемое поле судоку
         * @param row_index - индекс элемента строки
         * @param column_index - индекс элемента колонки
         * @param values, first_values, second_values, final_values - множества для записи их пересечения
         * @param val - локальное "универсальное" множество
         * @param row, col, group - значения соответсвующих полей
         *
         * @description Поиск подходящих значений происходит через пересечение наборов, полученных с
         * помощью готовых методов получения значений строк, столбцов и блоков
         * @return Values or set<int>
         * */
        Values findPossibleValues(int row_index, int column_index, Board& board) {
            Values values, first_result, second_result, final_result;
            Values val = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            Values row = Sudoku::getRowValues(row_index, board);
            Values col = Sudoku::getColumnValues(column_index, board);
            Values group = Sudoku::getGroupValues(row_index, column_index, board);
            set_difference(val.begin(), val.end(),
                           row.begin(), row.end(),
                           inserter(first_result, first_result.end()));
            set_difference(first_result.begin(), first_result.end(),
                           col.begin(), col.end(),
                           inserter(second_result, second_result.end()));
            set_difference(second_result.begin(), second_result.end(),
                           group.begin(), group.end(),
                           inserter(final_result, final_result.end()));
            return final_result;
        }
        /**
         * @name <h1>solution</h1>
         *
         * @param board - передоваемое поле судоку
         *
         * @description Метод, позволяющий запустить корректную рекурсивность в связке с методом Endgame,
         * и в случае возвращения true от метода Endgame, применять изменения элементов передающегося в него Board
         * @return Board or {}
         * */
        Board solution(const Board& board) {
            Board solution = board;
            if (Endgame(&solution)) {
                return solution;
            }
            return {};
        }
        /**
         * @name <h1>Endgame</h1>
         *
         * @param minRow - переменна для обозначения номера строки
         * @param minColumn - переменная для обозначения номера столбца
         * @param minValues - минимальное количество вариантов
         * @param possibleValues - множество подходящих значений для элемента
         * @param possibleValuesCount - количество подходящих значений для элемента
         *
         * @description Основной метод решения судоку, рекурсивно проходим по полю, выделяя доступные значения для
         * каждого элемента, вставляя элементы из множества действительных в пустые клетки по заданным условиям:
         * <div>
         *      <li>Если все клетки заполнены, <strong>то завершаем цикл и возвращаем найденное решение<strong/></li>
         *      <li>Иначе если ни одну клетку за проход заполнить не удалось, <strong>то завершаем цикл<strong/></li>
         *      <li>Для клетки с минимальным количеством вариантов:
         *          <br/><br/>
         *          Пробуем ставить каждую цифру по порядку и рекурсивно решать получившиеся Судоку
         *          <br/>
         *          Если решение было найдено, то возвращаем его
         *      </li>
         * </div>
         * @return bool Type of true/false
         * */
        bool Endgame(Board* solution) {
            int minRow = -1; int minColumn = -1;
            Values minValues;
            while (true) {
                    minRow = -1;
                    for (int row_index = 0; row_index < 9; row_index++) {
                        for (int column_index = 0; column_index < 9; column_index++) {
                            if (getPosition(row_index, column_index, *solution) != 0) continue;
                            Values possibleValues = findPossibleValues(row_index, column_index, *solution );
                            int possibleValuesCount = possibleValues.size();
                            if (possibleValuesCount == 0) return false;
                            if (possibleValuesCount == 1) {
                                setPosition(row_index, column_index, *possibleValues.begin(), *solution);
                            }
                            if (minRow < 0 || possibleValuesCount < minValues.size()) {
                                minRow = row_index;
                                minColumn = column_index;
                                minValues = possibleValues;
                            }
                        }
                    }
                    if (minRow == -1) {
                        return true;
                    } else if (1 < minValues.size()) {
                        break;
                    }
            }
            for (auto element : minValues) {
                Board solutionCopy = *solution;
                solutionCopy[minRow][minColumn] = element;
                if (Endgame(&solutionCopy)) {
                    *solution = solutionCopy;
                    return true;
                }
            }
            return false;
        }

        /**
        * @name <h1>printBoard</h1>
        *
        * @param board - передоваемое поле судоку
        *
        * @description Вывод поля судоку
        * @return Board or vector<vector<int>>
        * */
        void printBoard(const Board& board) {
            for (int row = 0; row < board.size(); row++) {
                for (int col = 0; col < board.size(); col++) {
                    if (col == 3 || col == 6) cout << " | ";
                    cout.width(3);
                    cout << board[row][col] << " ";
                }
                if (row == 2 || row == 5) {
                    cout << '\n';
                    for (int i = 0; i < board.size(); i++)
                        cout << "-----";
                }
                cout << '\n';
            }
        }

        /**
         * @name <h1>Run</h1>
         *
         * @param board - шаблонное поле судоку
         * @param solution - итоговое, решённое поле
         *
         * @description Метод, подводящий итог программы, для изначально сформированного Board вызывает метод для
         * решения, также метод вывода поля.
         * @return void
         * */
        void Run() {
            Board board = {
                    { 0, 0, 0, 0, 6, 0, 7, 0, 0 },
                    { 0, 5, 9, 0, 0, 0, 0, 0, 0 },
                    { 0, 1, 0, 2, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 1, 0, 0, 0, 0, 0 },
                    { 6, 0, 0, 5, 0, 0, 0, 0, 0 },
                    { 3, 0, 0, 0, 0, 0, 4, 6, 0 },
                    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0, 0, 9, 1 },
                    { 8, 0, 0, 7, 4, 0, 0, 0, 0 }
            };
            cout << "---------------> ORIGINAL <----------------" << '\n' << '\n';
            printBoard(board); cout << '\n';

            Board solution = Sudoku::solution(board);
            if (!solution.empty()) {
                cout << "---------------> SOLUTION <----------------" << '\n' << '\n';
                printBoard(solution);
            }
            else cout << "Doesn't have a solution";
        }
};

int main() {
    auto sudoku = new Sudoku();
    sudoku->Run();

    return 0;
}
