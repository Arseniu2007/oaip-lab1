#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void fillFile(FILE* file) {
    int n;
    printf("Введите количество элементов: ");
    scanf("%d", &n);

    printf("Введите элементы (натуральные числа):\n");
    for (int i = 0; i < n; i++) {
        int value;
        scanf("%d", &value);
        fprintf(file, "%d\n", value);
    }
}

void displayFile(FILE* file) {
    fseek(file, 0, SEEK_SET);  // Перемещаемся в начало файла
    int value;
    printf("\nСодержимое файла:\n");
    while (fscanf(file, "%d", &value) != EOF) {
        printf("%d ", value);
    }
    printf("\n");
}

void transportItems(FILE* file, int capacity) {
    fseek(file, 0, SEEK_SET);
    int weight, count = 0, total_weight = 0;
    while (fscanf(file, "%d", &weight) != EOF) {
        total_weight += weight;
        count++;
        if (total_weight > capacity) {
            break;
        }
    }
    printf("\nКоличество предметов, которых можно увезти за один раз: %d\n", count);
}

void transformAndSortFile(FILE* file, int a, int b) {
    FILE* tempFile = fopen("temp.txt", "w+");
    if (!tempFile) {
        printf("Не удалось открыть временный файл.\n");
        return;
    }

    fseek(file, 0, SEEK_SET);
    int weight;
    double k = (double)(b - a) / 1000.0;  // Предположим, что максимальная масса предмета 1000
    double b_offset = a;

    while (fscanf(file, "%d", &weight) != EOF) {
        double transformed = k * weight + b_offset;
        fprintf(tempFile, "%.2f\n", transformed);
    }

    fclose(file);
    fclose(tempFile);

    // Перезаписываем исходный файл отсортированными значениями
    tempFile = fopen("temp.txt", "r");
    if (!tempFile) {
        printf("Не удалось открыть временный файл для чтения.\n");
        return;
    }

    // Чтение значений и сортировка
    double* values = malloc(100 * sizeof(double));  // Предположим, что максимум 100 значений
    int count = 0;
    while (fscanf(tempFile, "%lf", &values[count]) != EOF) {
        count++;
    }

    // Сортировка значений
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (values[i] > values[j]) {
                double temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }

    // Записываем отсортированные значения в файл
    file = fopen("file.txt", "w");
    if (!file) {
        printf("Не удалось открыть файл для записи.\n");
        free(values);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%.2f\n", values[i]);
    }

    printf("\nОтсортированные и преобразованные данные:\n");
    for (int i = 0; i < count; i++) {
        printf("%.2f ", values[i]);
    }
    printf("\n");

    free(values);
    fclose(file);
    remove("temp.txt");  // Удаляем временный файл
}

int main() {
    char filename[256];

    // Запрашиваем имя файла с клавиатуры
    printf("Введите имя файла: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "a+");  // Открываем файл для записи и чтения
    if (!file) {
        printf("Не удалось открыть файл: %s\n", filename);
        return 1;
    }

    // Заполняем файл числами с клавиатуры
    fillFile(file);

    // Показываем содержимое файла
    displayFile(file);

    int capacity;
    printf("\nВведите грузоподъемность автомобиля: ");
    scanf("%d", &capacity);

    // Определяем, сколько предметов можно увезти за один раз
    transportItems(file, capacity);

    int a, b;
    printf("\nВведите диапазон для преобразования (a, b): ");
    scanf("%d %d", &a, &b);

    // Линейно преобразуем данные и сортируем их
    transformAndSortFile(file, a, b);

    fclose(file);
    return 0;
}

