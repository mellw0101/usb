#ifndef APPADDDIALOG_H
#define APPADDDIALOG_H

#include <QDirIterator>
#include <QPushButton>
#include <QBoxLayout>
#include <QListWidget>
#include <QDialog>
#include <QLineEdit>

class AppAddDialog : public QDialog {
public:
    QString appPath;
    AppAddDialog(QWidget *parent = nullptr) : QDialog(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        QLineEdit *searchEdit = new QLineEdit(this);
        QListWidget *listWidget = new QListWidget(this);
        QPushButton *addButton = new QPushButton("Add", this);
        QPushButton *cancelButton = new QPushButton("Cancel", this);


        layout->addWidget(searchEdit);
        layout->addWidget(listWidget);
        layout->addWidget(addButton);
        layout->addWidget(cancelButton);

        connect(searchEdit, &QLineEdit::textChanged, this, [listWidget](const QString &searchTerm) {
            QString USER = getenv("USER");
            listWidget->clear();
            QStringList directories = {
                "/usr/bin/",
                "/usr/share/applications/",
                "/home/" + USER + "/.local/share/applications"
            };

            for (const QString& dir : directories) {
                QDirIterator it(dir, QDir::Files);
                while (it.hasNext()) {
                    it.next();
                    QString filename = it.fileName();
                    if (filename.contains(searchTerm, Qt::CaseInsensitive)) {
                        // Make sure not to insert duplicates
                        QList<QListWidgetItem *> found = listWidget->findItems(filename.section("/", -1), Qt::MatchExactly);
                        if (found.empty()) {
                            listWidget->addItem(filename.section("/", -1));
                        }
                    }
                }
            }
        });

        connect(addButton, &QPushButton::clicked, this, [this, listWidget]() {
            QList<QListWidgetItem *> selectedItems = listWidget->selectedItems();
            if (!selectedItems.isEmpty()) {
                appPath = selectedItems.first()->text();
                // Emit a signal or perform an action to add the app
                accept();
            }
        });

        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        setWindowTitle("Add New App");
    }
};

#endif // APPADDDIALOG_H
