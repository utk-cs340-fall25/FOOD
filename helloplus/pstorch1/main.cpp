#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
        window.setWindowTitle("Greetings");

        QVBoxLayout *layout = new QVBoxLayout(&window);

        QLabel *prompt = new QLabel("Please enter your name:");
        QLineEdit *input = new QLineEdit();
        QPushButton *button = new QPushButton("Submit");
        QLabel *output = new QLabel();

        layout->addWidget(prompt);
        layout->addWidget(input);
        layout->addWidget(button);
        layout->addWidget(output);

        QObject::connect(button, &QPushButton::clicked, [&]() {
            QString name = input->text();
            output->setText("Hello, " + name + "!");
        });

        window.setLayout(layout);
        window.resize(400, 400);
        window.show();


    return app.exec();
}
