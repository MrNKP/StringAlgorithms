#ifndef STRINGALGORITHMS_H
#define STRINGALGORITHMS_H

#include <QMainWindow>

#include <QMessageBox>
#include <QElapsedTimer>
#include <QVector>
#include <QDebug>

namespace Ui {
class StringAlgorithms;
}

struct Arc;
struct Node;

struct Arc
{
    int indexStart, indexFinish;
    Node *vertex;
    int indexVertex;
};

struct Node
{
    Arc *arcs[26];
};

class StringAlgorithms : public QMainWindow
{
    Q_OBJECT

public:
    explicit StringAlgorithms(QWidget *parent = nullptr);
    ~StringAlgorithms();

private slots:
    void on_naiveAlgorithmPushButton_clicked();

    void on_PrefixesPushButton_clicked();

    void on_SuffixesPushButton_clicked();

    void on_naiveZBlocksPushButton_clicked();

    void on_prefixZBlocksPushButton_clicked();

    void on_suffixZBlocksPushButton_clicked();

    void on_kmpAlgorithmPushButton_clicked();

    void on_bmAlgorithmPushButton_clicked();

    void on_bm1AlgorithmPushButton_clicked();

    void on_krAlgorithmPushButton_clicked();

    void on_saAlgorithmPushButton_clicked();

    void on_safAlgorithmPushButton_clicked();

    void on_stAlgorithmPushButton_clicked();

private:
    Ui::StringAlgorithms *ui;

    int max(int a, int b);
    int StrSameLen(QString src, int len, int index1, int index2);
    int StrSameLenBackward(QString src, int index1, int index2);
    int CheckSequence(int srcLine[], int n, int position1, int position2);
    int BadSymbolShift(QVector<QVector<int>> posList, int badChar, int badPosition);
    int GoodSuffixShift(int nearestSuffixX[], int restrictedBorders[], int badPosition, int n);
    int GornerScheme(QString str, int len, int prescaler);

    void NaiveStringMatch(QString src, QString substr);
    int NaiveMaxBorder(QString src);
    void PrefixBorderArray(QString src, int prefixBorders[]);
    void SuffixBorderArray(QString src, int suffixBorders[]);
    void PrefixBorderArrayModified(QString src, int prefixBorders[], int prefixBordersModified[]);
    void BpToBpm(int prefixBorders[], int prefixBordersModified[], int n);
    void BpmToBp(int prefixBordersModified[], int prefixBorders[], int n);
    void BsToBsm(int suffixBorders[], int suffixBordersModified[], int n);
    void BsmToBs(int suffixBordersModified[], int suffixBorders[], int n);
    void NaiveZBlockValues(QString src, int zblocks[]);
    void PrefixZBlockValues(QString src, int zblocks[]);
    void SuffixZBlockValues(QString src, int zblocks[]);
    void ZpToBpm(int zblocks[], int prefixBordersModified[], int n);
    void ZpToBp(int zblocks[], int prefixBorders[], int n);
    void BpToZp(int prefixBorders[], int zblocks[], int n);
    void KnutMorrisPratt(QString src, QString substr);
    void InitAlphabet(QVector<char> &alphabet);
    void GenerateListOfPositions(QString str, QVector<char> alphabet, QVector<QVector<int>> &posList);
    void BoierMur(QString src, QString substr);
    void BsToNs(int suffixBorders[], int nearestSuffix[], int n);
    void BsToBr(int suffixBorders[], int restrictedBorders[], int n);
    void BoierMur(QString src, QString substr, bool heavy);
    void KarpRabin(QString src, QString substr, int prescaler);
    void ShiftAnd(QString src, QString substr);
    void ShiftAndFz(QString src, QString substr, int countSymbolsNotMatch);
    Arc *findArcInSuffixTree(QString src, QString substr, int len, Node *tree, int &indexSubstr, int &indexArc);
    void GetLeavesSuffixTree(Arc *startArc, int numberSymbols, QString &resultIndexes);
    Node *initVertexSuffixTree();
    Arc *initArcSuffixTree(Node *localNode, int indexSymbol, int startIndex, int finishIndex, Node *destVertex, int indexDestVertex);
    Node *initSuffixTree(QString src);
};

#endif // STRINGALGORITHMS_H
