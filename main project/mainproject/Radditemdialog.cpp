#include "Radditemdialog.h"
#include "ui_Radditemdialog.h"
#include <QDialogButtonBox>
#include <QDialog>

RAddItemDialog::RAddItemDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RAddItemDialog) {
ui->setupUi(this);
connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
RAddItemDialog::~RAddItemDialog() { delete ui; }

void RAddItemDialog::setInitial(const RInventoryItem &it) {
ui->nameLineEdit->setText(it.name);
ui->qtySpin->setValue(it.qty);
ui->unitLineEdit->setText(it.unit);
ui->tagsLineEdit->setText(it.tags.join(", "));
}

RInventoryItem RAddItemDialog::resultItem() const {
RInventoryItem it;
it.name = ui->nameLineEdit->text().trimmed();
it.qty = ui->qtySpin->value();
it.unit = ui->unitLineEdit->text().trimmed();
QStringList tags = ui->tagsLineEdit->text().split(',', Qt::SkipEmptyParts);
for (auto &t : tags) it.tags.append(t.trimmed().toLower());
return it;
}
