#include <QtTest/QtTest>
#include <QApplication>
#include <QCheckBox>
#include <QSet>

#include "Rrsearchwindow.h"
#include "food.h"

class TestRRSearchWindow : public QObject {
    Q_OBJECT

private slots:
    void testOwnedFilter()
    {
        // Create widget (QApplication will be created by QTEST_MAIN)
        RRSearchWindow w;

        std::map<QString, Recipe> recipes;
        Recipe a; a.name = "Pancakes"; a.ingredients.push_back({"flour", "2 cups"}); a.ingredients.push_back({"egg","2"});
        Recipe b; b.name = "SaltSnack"; b.ingredients.push_back({"salt","1 tsp"});

        recipes[a.name] = a;
        recipes[b.name] = b;

        w.setRecipes(recipes);

        // both recipes should be visible initially
        QStringList visible = w.visibleRecipes();
        QCOMPARE(visible.size(), 2);
        QVERIFY(visible.contains("Pancakes"));
        QVERIFY(visible.contains("SaltSnack"));

        // Mark only flour & egg as owned
        QSet<QString> owned; owned << "flour" << "egg";
        w.setOwnedIngredients(owned);

        // toggle owned-only filter on
        QCheckBox *ownedCb = w.findChild<QCheckBox*>("ownedFilterCheckBox");
        QVERIFY(ownedCb != nullptr);
        ownedCb->setChecked(true);

        // Only Pancakes should remain
        visible = w.visibleRecipes();
        QVERIFY(visible.contains("Pancakes"));
        QVERIFY(!visible.contains("SaltSnack"));
    }
};

QTEST_MAIN(TestRRSearchWindow)
#include "test_rrsearchwindow.moc"
