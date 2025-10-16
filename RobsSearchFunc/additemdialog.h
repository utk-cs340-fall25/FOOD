#pragma once
#include <QDialog>
#include "inventorymodel.h"


namespace Ui { class AddItemDialog; }


class AddItemDialog : public QDialog {
Q_OBJECT
public:
explicit AddItemDialog(QWidget* parent = nullptr);
~AddItemDialog() override;
void setInitial(const InventoryItem &it);
InventoryItem resultItem() const;


private:
Ui::AddItemDialog *ui;
};
