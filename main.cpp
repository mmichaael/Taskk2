#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <QtWidgets>

class FileVisualizer : public QWidget {
public:
    FileVisualizer(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Графічна візуалізація файлів");
        resize(500, 300);

        // Кнопки
        openButton = new QPushButton("Відкрити файл", this);
        closeButton = new QPushButton("Закрити вкладку", this);
        saveBinaryButton = new QPushButton("Зберегти бінарний масив", this);
        saveDecimalButton = new QPushButton("Зберегти десяткове представлення", this);

        // Поля введення для вказання шляху до файлу
        filePathEdit = new QLineEdit(this);
        fileSizeLabel = new QLabel(this);
        fileSizeLabel->setText("Розмір файлу: ");

        // Графічна область для відображення масиву
        arrayGraphics = new QLabel(this);
        arrayGraphics->setFixedSize(300, 200);

        // Розміщення віджетів
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(filePathEdit);
        layout->addWidget(openButton);
        layout->addWidget(fileSizeLabel);
        layout->addWidget(arrayGraphics);
        layout->addWidget(saveBinaryButton);
        layout->addWidget(saveDecimalButton);
        layout->addWidget(closeButton);

        // З'єднання сигналів і слотів
        connect(openButton, &QPushButton::clicked, this, &FileVisualizer::openFile);
        connect(saveBinaryButton, &QPushButton::clicked, this, &FileVisualizer::saveBinaryArray);
        connect(saveDecimalButton, &QPushButton::clicked, this, &FileVisualizer::saveDecimalArray);
        connect(closeButton, &QPushButton::clicked, this, &FileVisualizer::close);
    }

private slots:
    void openFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Виберіть файл", QDir::homePath(), "Текстові файли (*.txt)");
        if (!fileName.isEmpty()) {
            filePathEdit->setText(fileName);
            visualizeArray(fileName.toStdString());
            showFileSize(fileName);
        }
    }

    void saveBinaryArray() {
        QString fileName = QFileDialog::getSaveFileName(this, "Зберегти бінарний масив", QDir::homePath(), "Binary files (*.bin)");
        if (!fileName.isEmpty()) {
            std::ofstream outFile(fileName.toStdString(), std::ios::binary);
            if (outFile.is_open()) {
                outFile.write(reinterpret_cast<const char*>(&binaryArray[0]), binaryArray.size());
                outFile.close();
            }
        }
    }

    void saveDecimalArray() {
        QString fileName = QFileDialog::getSaveFileName(this, "Зберегти десяткове представлення", QDir::homePath(), "Text files (*.txt)");
        if (!fileName.isEmpty()) {
            std::ofstream outFile(fileName.toStdString());
            if (outFile.is_open()) {
                for (int i : binaryArray) {
                    outFile << i << std::endl;
                }
                outFile.close();
            }
        }
    }

    void visualizeArray(const std::string& fileName) {
        std::ifstream file(fileName);
        if (file.is_open()) {
            std::vector<int> array;
            int value;
            while (file >> value) {
                array.push_back(value);
            }
            file.close();

            int totalPixels = 300 * 200;
            int arraySize = array.size();
            binaryArray.clear();

            QImage image(300, 200, QImage::Format_RGB32);
            image.fill(Qt::gray);

            for (int i = 0; i < totalPixels; ++i) {
                int pixelValue = (i < arraySize) ? array[i] : 2;
                if (pixelValue == 1) {
                    image.setPixel(i % 300, i / 300, qRgb(0, 0, 0));
                    binaryArray.push_back(1);
                } else if (pixelValue == 0) {
                    image.setPixel(i % 300, i / 300, qRgb(255, 255, 255));
                    binaryArray.push_back(0);
                } else {
                    binaryArray.push_back(2); // gray
                }
            }

            arrayGraphics->setPixmap(QPixmap::fromImage(image));
        }
    }

    void showFileSize(const QString& fileName) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QFileInfo fileInfo(file);
            fileSizeLabel->setText("Розмір файлу: " + QString::number(fileInfo.size()) + " байт");
            file.close();
        }
    }

private:
    QPushButton *openButton;
    QPushButton *closeButton;
    QPushButton *saveBinaryButton;
    QPushButton *saveDecimalButton;
    QLineEdit *filePathEdit;
    QLabel *fileSizeLabel;
    QLabel *arrayGraphics;
    std::vector<int> binaryArray;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    FileVisualizer visualizer;
    visualizer.show();
    return app.exec();
}
