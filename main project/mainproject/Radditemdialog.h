#pragma once
#include <QDialog>
#include "Rinventorymodel.h"

namespace Ui { class RAddItemDialog; }

class RAddItemDialog : public QDialog {
Q_OBJECT
public:
explicit RAddItemDialog(QWidget* parent = nullptr);
~RAddItemDialog() override;
void setInitial(const RInventoryItem &it);
RInventoryItem resultItem() const;

private:
Ui::RAddItemDialog *ui;
};
