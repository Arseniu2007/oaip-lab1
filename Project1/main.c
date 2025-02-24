#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// Функция очистки буфера ввода при ошибке
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Функция безопасного ввода целого числа
int safeInputInt(const char* prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            return value;  // Если ввод успешен, возвращаем значение
        }
        else {
            printf("Ошибка: Вы ввели неверные данные. Попробуйте снова.\n");
            clearInputBuffer(); // Очищаем ввод
        }
    }
}

// Заполнение файла числами
void fillFile(FILE* file) {
    int n = safeInputInt("Введите количество элементов: ");
    printf("Введите элементы (натуральные числа):\n");
    for (int i = 0; i < n; i++) {
        int value = safeInputInt("");  // Запрашиваем числа без дополнительного текста
        fprintf(file, "%d\n", value);
    }
}

// Отображение содержимого файла
void displayFile(FILE* file) {
    fseek(file, 0, SEEK_SET);
    int value;
    printf("\nСодержимое файла:\n");
    while (fscanf(file, "%d", &value) == 1) {
        printf("%d ", value);
    }
    printf("\n");
}

// Определение количества перевозимых предметов
void transportItems(FILE* file, int capacity) {
    fseek(file, 0, SEEK_SET);
    int weight, count = 0, total_weight = 0;
    while (fscanf(file, "%d", &weight) == 1) {
        total_weight += weight;
        count++;
        if (total_weight > capacity) {
            break;
        }
    }
    printf("\nКоличество предметов, которых можно увезти за один раз: %d\n", count);
}

// Линейное преобразование и сортировка данных
void transformAndSortFile(FILE* file, int a, int b) {
    FILE* tempFile = fopen("temp.txt", "w+");
    if (!tempFile) {
        printf("Ошибка: Не удалось открыть временный файл.\n");
        return;
    }

    fseek(file, 0, SEEK_SET);
    int weight;
    double k = (double)(b - a) / 1000.0;  // Предполагаем, что max вес = 1000
    double b_offset = a;

    while (fscanf(file, "%d", &weight) == 1) {
        double transformed = k * weight + b_offset;
        fprintf(tempFile, "%.2f\n", transformed);
    }

    fclose(file);
    fclose(tempFile);

    // Читаем из временного файла и сортируем
    tempFile = fopen("temp.txt", "r");
    if (!tempFile) {
        printf("Ошибка: Не удалось открыть временный файл для чтения.\n");
        return;
    }

    double* values = malloc(100 * sizeof(double));
    int count = 0;
    while (fscanf(tempFile, "%lf", &values[count]) == 1) {
        count++;
    }

    // Простая пузырьковая сортировка
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (values[i] > values[j]) {
                double temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }

    // Запись в файл
    file = fopen("file.txt", "w");
    if (!file) {
        printf("Ошибка: Не удалось открыть файл для записи.\n");
        free(values);
        return;
    }

    printf("\nОтсортированные и преобразованные данные:\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%.2f\n", values[i]);
        printf("%.2f ", values[i]);
    }
    printf("\n");

    free(values);
    fclose(file);
    remove("temp.txt");  // Удаляем временный файл
}

// Главная функция
int main() {
    char filename[256];

    printf("Введите имя файла: ");
    scanf("%255s", filename);
    clearInputBuffer();  // Очистка буфера после строки

    FILE* file = fopen(filename, "a+");
    if (!file) {
        printf("Ошибка: Не удалось открыть файл: %s\n", filename);
        return 1;
    }

    fillFile(file);
    displayFile(file);

    int capacity = safeInputInt("\nВведите грузоподъемность автомобиля: ");
    transportItems(file, capacity);

    int a = safeInputInt("\nВведите начало диапазона преобразования (a): ");
    int b = safeInputInt("Введите конец диапазона преобразования (b): ");

    transformAndSortFile(file, a, b);
    fclose(file);
    return 0;
}
