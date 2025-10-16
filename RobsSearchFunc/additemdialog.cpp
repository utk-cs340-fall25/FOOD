#include "additemdialog.h"
#include "ui_additemdialog.h"


AddItemDialog::AddItemDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AddItemDialog) {
ui->setupUi(this);
connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
AddItemDialog::~AddItemDialog() { delete ui; }


void AddItemDialog::setInitial(const InventoryItem &it) {
ui->nameLineEdit->setText(it.name);
ui->qtySpin->setValue(it.qty);
ui->unitLineEdit->setText(it.unit);
ui->tagsLineEdit->setText(it.tags.join(", "));
}


InventoryItem AddItemDialog::resultItem() const {
InventoryItem it;
it.name = ui->nameLineEdit->text().trimmed();
it.qty = ui->qtySpin->value();
it.unit = ui->unitLineEdit->text().trimmed();
QStringList tags = ui->tagsLineEdit->text().split(',', Qt::SkipEmptyParts);
for (auto &t : tags) it.tags.append(t.trimmed().toLower());
return it;
}
