#include "stringalgorithms.h"
#include "ui_stringalgorithms.h"

StringAlgorithms::StringAlgorithms(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StringAlgorithms)
{
    ui->setupUi(this);
}

StringAlgorithms::~StringAlgorithms()
{
    delete ui;
}

void StringAlgorithms::NaiveStringMatch(QString src, QString substr)
{
    QElapsedTimer timer;
    timer.start();
    int n = src.size();
    int m = substr.size();
    QString resultIndexes;
    resultIndexes = "";
    for (int i = 0; i <= n-m; i++)
    {
        int j = 0;
        while (j < m && substr[j] == src[i+j])
            j++;
        if (j == m)
        {
            resultIndexes += "Найдено вхождение в позиции : ";
            resultIndexes += QString::number(i);
            resultIndexes += "\n";
        }
    }
    ui->statusBar->showMessage("Naive Time = " + QString::number(timer.elapsed()) + " ms");
    if (resultIndexes != "")
        QMessageBox::information(this, "Result", resultIndexes, QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Result", "Вхождений не найдено", QMessageBox::Ok);
}

int StringAlgorithms::NaiveMaxBorder(QString src)
{
    int n = src.size();
    int br = 0;
    for (int i = n-1; !br && i; i--)
    {
        int j=0;
        while (j < i && src[j] == src[n-i+j])
            j++;
        if (j == i)
            br = i;
    }
    return br;
}

void StringAlgorithms::PrefixBorderArray(QString src, int prefixBorders[])
{
    int n = src.size();
    prefixBorders[0] = 0;
    int rightPrefixBorders;
    for (int i = 1; i < n; i++)
    {
        rightPrefixBorders = prefixBorders[i-1];
        while (rightPrefixBorders && (src[i] != src[rightPrefixBorders]))
            rightPrefixBorders = prefixBorders[rightPrefixBorders - 1];
        if (src[i] == src[rightPrefixBorders])
            prefixBorders[i] = rightPrefixBorders + 1;
        else
            prefixBorders[i] = 0;
    }
}

void StringAlgorithms::SuffixBorderArray(QString src, int suffixBorders[])
{
    int n = src.size();
    suffixBorders[n-1] = 0;
    int leftSuffixBorders;
    for (int i = n - 2; i >= 0; i--)
    {
        leftSuffixBorders = suffixBorders[i + 1];
        while (leftSuffixBorders && (src[i] != src[n - leftSuffixBorders - 1]))
            leftSuffixBorders = suffixBorders[n - leftSuffixBorders];
        if (src[i] == src[n - leftSuffixBorders - 1])
            suffixBorders[i] = leftSuffixBorders + 1;
        else
            suffixBorders[i] = 0;
    }
}

void StringAlgorithms::PrefixBorderArrayModified(QString src, int prefixBorders[], int prefixBordersModified[])
{
    int n = src.size();
    prefixBordersModified[0] = 0;
    prefixBordersModified[n-1] = prefixBorders[n-1];
    for (int i = 1; i < n-1; i++)
        if (prefixBorders[i] && (src[prefixBorders[i]] == src[i+1]))
            prefixBordersModified[i] = prefixBordersModified[prefixBorders[i] - 1];
        else
            prefixBordersModified[i] = prefixBorders[i];
}

void StringAlgorithms::BpToBpm(int prefixBorders[], int prefixBordersModified[], int n)
{
    prefixBordersModified[0] = 0;
    prefixBordersModified[n-1] = prefixBorders[n-1];
    for (int i = 1; i < n-1; i++)
        if (prefixBorders[i] && (prefixBorders[i] + 1 == prefixBorders[i+1]))
            prefixBordersModified[i] = prefixBordersModified[prefixBorders[i] - 1];
        else
            prefixBordersModified[i] = prefixBorders[i];
}

void StringAlgorithms::BpmToBp(int prefixBordersModified[], int prefixBorders[], int n)
{
    prefixBorders[n-1] = prefixBordersModified[n-1];
    prefixBorders[0] = 0;
    for (int i = n-2; i>0; i--)
        prefixBorders[i] = max(prefixBorders[i+1] - 1, prefixBordersModified[i]);
}

void StringAlgorithms::BsToBsm(int suffixBorders[], int suffixBordersModified[], int n)
{
    suffixBordersModified[n-1] = 0;
    suffixBordersModified[0] = suffixBorders[0];
    for (int i = n-2; i>0; i--)
        if (suffixBorders[i] && (suffixBorders[i] + 1 == suffixBorders[i-1]))
            suffixBordersModified[i] = suffixBordersModified[n-suffixBorders[i]];
        else
            suffixBordersModified[i] = suffixBorders[i];
}

void StringAlgorithms::BsmToBs(int suffixBordersModified[], int suffixBorders[], int n)
{
    suffixBorders[0] = suffixBordersModified[0];
    suffixBorders[n-1] = 0;
    for (int i = 1; i < n-1; i++)
        suffixBorders[i] = max(suffixBorders[i-1] - 1, suffixBordersModified[i]);
}

void StringAlgorithms::NaiveZBlockValues(QString src, int zblocks[])
{
    int n = src.size();
    zblocks[0] = 0;
    for (int i=1; i<n; i++)
    {
        int j=i;
        while (j<n && src[j] == src[j-i])
            j++;
        zblocks[i] = j-i;
    }
}

void StringAlgorithms::PrefixZBlockValues(QString src, int zblocks[])
{
    int n = src.size();
    int left = 0, right = 0;
    zblocks[0] = 0;
    for (int i=1; i<n; i++)
    {
        zblocks[i] = 0;
        if (i >= right)
        {
            zblocks[i] = StrSameLen(src, n, 0, i);
            left = i;
            right = left + zblocks[i];
        }
        else
        {
            int j = i - left;
            if (zblocks[j] < right - i)
                zblocks[i] = zblocks[j];
            else
            {
                zblocks[i] = right - i + StrSameLen(src, n, right - i, right);
                left = i;
                right = left + zblocks[i];
            }
        }
    }
}

void StringAlgorithms::SuffixZBlockValues(QString src, int zblocks[])
{
    int n = src.size();
    int left = n - 1, right = n - 1;
    zblocks[n-1] = 0;
    for (int i = n-2; i>=0; i--)
    {
        zblocks[i] = 0;
        if (i <= left)
        {
            zblocks[i] = StrSameLenBackward(src, i, n-1);
            right = i;
            left = right - zblocks[i];
        }
        else
        {
            int j = n - (right + 1 - i);
            if (zblocks[j] < i - left)
                zblocks[i] = zblocks[j];
            else
            {
                zblocks[i] = i - left + StrSameLenBackward(src, left, n - i + left);
                right = i;
                left = right - zblocks[i];
            }
        }
    }
}

void StringAlgorithms::ZpToBpm(int zblocks[], int prefixBordersModified[], int n)
{
    for (int i=0; i<n; i++)
        prefixBordersModified[i] = 0;
    for (int j = n-1; j>=0; j--)
    {
        int i = j + zblocks[j] - 1;
        prefixBordersModified[i] = zblocks[j];
    }
}

void StringAlgorithms::ZpToBp(int zblocks[], int prefixBorders[], int n)
{
    for (int i = 0; i<n; i++)
        prefixBorders[i] = 0;
    for (int j = 1; j<n; j++)
        for (int i = j+zblocks[j] - 1; i >= j; i--)
        {
            if (prefixBorders[i]) break;
            prefixBorders[i] = i - j + 1;
        }
}

void StringAlgorithms::BpToZp(int prefixBorders[], int zblocks[], int n)
{
    int left = 0, right = 0;
    zblocks[0] = 0;
    for (int i=1; i<n; i++)
    {
        zblocks[i] = 0;
        if (i >= right)
        {
            zblocks[i] = CheckSequence(prefixBorders, n, i, 1);
            left = i;
            right = left + zblocks[i];
        }
        else
        {
            int j = i - left;
            if (zblocks[j] < right - i)
                zblocks[i] = zblocks[j];
            else
            {
                zblocks[i] = right - i + CheckSequence(prefixBorders, n, right, right - i + 1);
                left = i;
                right = left + zblocks[i];
            }
        }
    }
}

void StringAlgorithms::KnutMorrisPratt(QString src, QString substr)
{
    QElapsedTimer timer;
    timer.start();
    int srcL = src.size();
    int substrL = substr.size();
    int pba[srcL], pbam[srcL];
    PrefixBorderArray(substr, pba);
    BpToBpm(pba, pbam, substrL);
    int k = 0;
    QString resultIndexes = "";
    for (int i = 0; i<srcL; i++)
    {
        while (k && substr[k] != src[i])
            k = pbam[k-1];
        if (substr[k] == src[i])
            k++;
        if (k == substrL)
        {
            resultIndexes += "Найдено вхождение в позиции : ";
            resultIndexes += QString::number(i - k + 1);
            resultIndexes += "\n";
            k = pbam[k-1];
        }
    }
    ui->statusBar->showMessage("KMP Time = " + QString::number(timer.elapsed()) + " ms");
    if (resultIndexes != "")
        QMessageBox::information(this, "Result", resultIndexes, QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Result", "Вхождений не найдено", QMessageBox::Ok);
}

void StringAlgorithms::InitAlphabet(QVector<char> &alphabet)
{
    for (char i='a'; i<='z'; i++)
        alphabet.append(i);
}

void StringAlgorithms::GenerateListOfPositions(QString str, QVector<char> alphabet, QVector<QVector<int>> &posList)
{
    int alphabetL = alphabet.size(), strL = str.size();
    char firstSymbol = alphabet.at(0);
    for (int i = 0; i<alphabetL; i++)
        posList.append(QVector<int>());
    for (int i = strL-1; i>=0; i--)
    {
        char tmp = str[i].toLatin1();
        int startIndex = tmp - firstSymbol;
        posList[startIndex].append(i);
    }
}

int StringAlgorithms::BadSymbolShift(QVector<QVector<int>> posList, int badChar, int badPosition)
{
    if (badPosition < 0)
        return 1;
    int neededPositionNumber = -1;
    QVector<int> localList = posList[badChar];
    int neededLength = localList.size();
    if (neededLength > 0)
        for (int i = 0; i<neededLength; i++)
            if (localList.at(i) < badPosition)
            {
                neededPositionNumber = localList.at(i);
                break;
            }
    return badPosition - neededPositionNumber;
}

void StringAlgorithms::BoierMur(QString src, QString substr)
{
    QElapsedTimer timer;
    timer.start();
    QString resultIndexes = "";
    QVector<char> alphabet;
    InitAlphabet(alphabet);
    QVector<QVector<int>> positionLists;
    GenerateListOfPositions(substr, alphabet, positionLists);
    int srcL = src.size(), substrL = substr.size();
    int numberOfTextRight = substrL;
    while (numberOfTextRight <= srcL)
    {
        int k = substrL - 1;
        int i = numberOfTextRight - 1;
        for (; k>=0; k--, i--)
            if (substr.at(k) != src.at(i))
                break;
        if (k < 0)
        {
            resultIndexes += "Найдено вхождение в позиции : ";
            resultIndexes += QString::number(i + 1);
            resultIndexes += "\n";
        }
        //numberOfTextRight += BadSymbolShift(positionLists, src[i].toLatin1()-alphabet[0], k);
        if (i < 0)
            numberOfTextRight += BadSymbolShift(positionLists, alphabet.indexOf(src[i+1].toLatin1()), k);
        else
            numberOfTextRight += BadSymbolShift(positionLists, alphabet.indexOf(src[i].toLatin1()), k);
    }
    ui->statusBar->showMessage("BM Time = " + QString::number(timer.elapsed()) + " ms");
    if (resultIndexes != "")
        QMessageBox::information(this, "Result", resultIndexes, QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Result", "Вхождений не найдено", QMessageBox::Ok);
}

void StringAlgorithms::BsToNs(int suffixBorders[], int nearestSuffix[], int n)
{
    for (int i=0; i<n; i++)
        nearestSuffix[i] = -1;
    for (int i=0; i<n-1; i++)
        if (suffixBorders[i])
        {
            int k = n - suffixBorders[i] - 1;
            nearestSuffix[k] = i;
        }
}

void StringAlgorithms::BsToBr(int suffixBorders[], int restrictedBorders[], int n)
{
    int currentBorder = suffixBorders[0], k = 0;
    while (currentBorder)
    {
        for (; k < n - currentBorder; k++)
            restrictedBorders[k] = currentBorder;
        currentBorder = suffixBorders[k];
    }
    for (; k < n; k++)
        restrictedBorders[k] = 0;
}

int StringAlgorithms::GoodSuffixShift(int nearestSuffixX[], int restrictedBorders[], int badPosition, int n)
{
    if (badPosition == n-1)
        return 1;
    if (badPosition < 0)
        return n - restrictedBorders[0];
    int copyPosition = nearestSuffixX[badPosition];
    if (copyPosition >= 0)
        return badPosition - copyPosition + 1;
    else
        return n - restrictedBorders[badPosition];
}

int StringAlgorithms::GornerScheme(QString str, int len, int prescaler)
{
    int result = 0;
    for (int i=0; i<len; i++)
        result = (result * 2 + str.at(i).toLatin1()) % prescaler;
    return result;
}

void StringAlgorithms::BoierMur(QString src, QString substr, bool heavy)
{
    QElapsedTimer timer;
    timer.start();
    QString resultIndexes = "";
    QVector<char> alphabet;
    InitAlphabet(alphabet);
    QVector<QVector<int>> positionLists;
    GenerateListOfPositions(substr, alphabet, positionLists);
    int srcL = src.size(), substrL = substr.size();

//    QString results = "";
    int suffixBorders[substrL];
    SuffixBorderArray(substr, suffixBorders);
    int restrictedBorders[substrL];
    BsToBr(suffixBorders, restrictedBorders, substrL);
    int nearestSuffixesX[substrL];
    if (heavy)
    {
        int suffixBordersModified[substrL];
        BsToBsm(suffixBorders, suffixBordersModified, substrL);
        BsToNs(suffixBordersModified, nearestSuffixesX, substrL);
//        results += "suffixBorders = ";
//        for (int i=0; i<substrL; i++)
//        {
//            results += QString::number(suffixBorders[i]);
//            results += " ";
//        }
//        results += "\n";
//        results += "suffixBordersModified = ";
//        for (int i=0; i<substrL; i++)
//        {
//            results += QString::number(suffixBordersModified[i]);
//            results += " ";
//        }
//        results += "\n";
//        results += "nearestSuffixesX = ";
//        for (int i=0; i<substrL; i++)
//        {
//            results += QString::number(nearestSuffixesX[i]);
//            results += " ";
//        }
//        results += "\n";
//        results += "restrictedBorders = ";
//        for (int i=0; i<substrL; i++)
//        {
//            results += QString::number(restrictedBorders[i]);
//            results += " ";
//        }
    }
    else
    {
        BsToNs(suffixBorders, nearestSuffixesX, substrL);
//        results += "suffixBorders = ";
//        for (int i=0; i<substrL; i++)
//        {
//            results += QString::number(suffixBorders[i]);
//            results += " ";
//        }
//        results += "\n";
//        results += "nearestSuffixesX = ";
//        for (int i=0; i<substrL; i++)
//        {
//            results += QString::number(nearestSuffixesX[i]);
//            results += " ";
//        }
//        results += "\n";
//        results += "restrictedBorders = ";
//        for (int i=0; i<substrL; i++)
//        {
//            results += QString::number(restrictedBorders[i]);
//            results += " ";
//        }
    }

    int numberOfTextRight = substrL;
    int shiftSymbol, shiftSuffix;
    while (numberOfTextRight <= srcL)
    {
        int k = substrL - 1;
        int i = numberOfTextRight - 1;
        for (; k>=0; k--, i--)
            if (substr.at(k) != src.at(i))
                break;
        if (k < 0)
        {
            resultIndexes += "Найдено вхождение в позиции : ";
            resultIndexes += QString::number(i + 1);
            resultIndexes += "\n";
        }
        if (i < 0)
            shiftSymbol = BadSymbolShift(positionLists, alphabet.indexOf(src[i+1].toLatin1()), k);
        else
            shiftSymbol = BadSymbolShift(positionLists, alphabet.indexOf(src[i].toLatin1()), k);
        shiftSuffix = GoodSuffixShift(nearestSuffixesX, restrictedBorders, k, substrL);
        numberOfTextRight += max(shiftSymbol, shiftSuffix);
    }
    ui->statusBar->showMessage("BM1 Time = " + QString::number(timer.elapsed()) + " ms");
//    QMessageBox::information(this, "Borders", results, QMessageBox::Ok);
    if (resultIndexes != "")
        QMessageBox::information(this, "Result", resultIndexes, QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Result", "Вхождений не найдено", QMessageBox::Ok);
}

void StringAlgorithms::KarpRabin(QString src, QString substr, int prescaler)
{
    QElapsedTimer timer;
    timer.start();
    int srcL = src.size();
    int substrL = substr.size();
    src += '\0';
    int power2 = 1;
    for (int i=0; i<substrL-1; i++)
        power2 = (power2 * 2) % prescaler;
    int hashSubstr = GornerScheme(substr, substrL, prescaler);
    int hashSrc = GornerScheme(src, substrL, prescaler);
    QString resultIndexes = "";
    for (int i=0; i<=srcL-substrL; i++)
    {
        if (hashSubstr == hashSrc)
        {
            int k = 0;
            while (k < substrL && substr.at(k) == src.at(i+k))
                k++;
            if (k == substrL)
            {
                resultIndexes += "Найдено вхождение в позиции : ";
                resultIndexes += QString::number(i);
                resultIndexes += "\n";
            }
        }
        hashSrc = ((hashSrc - power2 * src.at(i).toLatin1()) * 2 + src.at(i + substrL).toLatin1()) % prescaler;
        if (hashSrc < 0)
            hashSrc += prescaler;
    }
    ui->statusBar->showMessage("KR Time = " + QString::number(timer.elapsed()) + " ms");
    if (resultIndexes != "")
        QMessageBox::information(this, "Result", resultIndexes, QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Result", "Вхождений не найдено", QMessageBox::Ok);
}

void StringAlgorithms::ShiftAnd(QString src, QString substr)
{
    QElapsedTimer timer;
    timer.start();
    QString resultIndexes = "";
    int srcL = src.size();
    int substrL = substr.size();
    QChar beginning = 'a';
    QChar conclusion = 'z';
    int countSymbols = conclusion.toLatin1() - beginning.toLatin1() + 1;
    QVector<int> bitmap(countSymbols, 0);
    for (int i=0; i<substrL; i++)
        bitmap[substr.at(i).toLatin1() - beginning.toLatin1()] |= 1 << (substrL - 1 - i);
    int upperOne = 1 << (substrL - 1);
    int rez = 0;
    int deb;
    int one = 1;
    for (int i=0; i<srcL; i++)
    {
        rez = (rez >> 1 | upperOne) & bitmap.at(src.at(i).toLatin1() - beginning.toLatin1());
        deb = rez & one;
        if (rez & one)
        {
            resultIndexes += "Найдено вхождение в позиции : ";
            resultIndexes += QString::number(i - substrL + 1);
            resultIndexes += "\n";
        }
    }
    ui->statusBar->showMessage("SA Time = " + QString::number(timer.elapsed()) + " ms");
    if (resultIndexes != "")
        QMessageBox::information(this, "Result", resultIndexes, QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Result", "Вхождений не найдено", QMessageBox::Ok);
}

void StringAlgorithms::ShiftAndFz(QString src, QString substr, int countSymbolsNotMatch)
{
    QElapsedTimer timer;
    timer.start();
    QString resultIndexes = "";
    int srcL = src.size();
    int substrL = substr.size();
    QChar beginning = 'a';
    QChar conclusion = 'z';
    int countSymbols = conclusion.toLatin1() - beginning.toLatin1() + 1;
    QVector<int> bitmap(countSymbols, 0);
    for (int i=0; i<substrL; i++)
        bitmap[substr.at(i).toLatin1() - beginning.toLatin1()] |= 1 << (substrL - 1 - i);
    int upperOne = 1 << (substrL - 1);
    QVector<int> rez(countSymbolsNotMatch+1, 0), rez1(countSymbolsNotMatch+1, 0);
    for (int i=0; i<srcL; i++)
    {
        for (int j = 0; j<=countSymbolsNotMatch; j++)
        {
            rez1[j] = rez[j];
            rez[j] = (rez[j] >> 1 | upperOne) & bitmap.at(src.at(i).toLatin1() - beginning.toLatin1());
            if (j)
                rez[j] |= (rez1[j-1] >> 1 | upperOne);
            if (j == countSymbolsNotMatch && rez.at(j) & 1)
            {
                resultIndexes += "Найдено вхождение в позиции : ";
                resultIndexes += QString::number(i - substrL + 1);
                resultIndexes += "\n";
            }
        }
    }
    ui->statusBar->showMessage("SAF Time = " + QString::number(timer.elapsed()) + " ms");
    if (resultIndexes != "")
        QMessageBox::information(this, "Result", resultIndexes, QMessageBox::Ok);
    else
        QMessageBox::critical(this, "Result", "Вхождений не найдено", QMessageBox::Ok);
}

Arc *StringAlgorithms::findArcInSuffixTree(QString src, QString substr, int len, Node *tree, int &indexSubstr, int &indexArc)
{
    QChar beginning = 'a';
    int substrL = substr.size();
    Arc *resultArc = nullptr;
    indexSubstr = indexArc = 0;
    Node *current = tree;
    bool stopFlag = false;
    while (!stopFlag && current)
    {
        Arc *newArc = current->arcs[substr.at(indexSubstr).toLatin1() - beginning.toLatin1()];
        if (newArc)
        {
            resultArc = newArc;
            indexArc = resultArc->indexStart;
            while ((indexSubstr < len) && (indexArc < resultArc->indexFinish + 1) && (substr.at(indexSubstr) == src.at(indexArc)))
            {
                indexSubstr++;
                indexArc++;
            }
            if (indexArc <= resultArc->indexFinish)
                stopFlag = true;
            else
                current = resultArc->vertex;
        }
        else
            stopFlag = true;
    }
    if (indexSubstr == substrL)
        indexArc++;
    return resultArc;
}

Node *StringAlgorithms::initVertexSuffixTree()
{
    Node *newVertex = new Node();
    for (int i=0; i<26; i++)
        newVertex->arcs[i] = nullptr;
    return newVertex;
}

Arc *StringAlgorithms::initArcSuffixTree(Node *localNode, int indexSymbol, int startIndex, int finishIndex, Node *destVertex, int indexDestVertex)
{
    Arc *newArc = new Arc();
    newArc->indexStart = startIndex;
    newArc->indexFinish = finishIndex;
    newArc->vertex = destVertex;
    newArc->indexVertex = indexDestVertex;
    localNode->arcs[indexSymbol] = newArc;
    return newArc;
}

Node *StringAlgorithms::initSuffixTree(QString src)
{
    Node *newTree = nullptr;
    int srcL = src.size();
    QChar start = 'a';
    newTree = initVertexSuffixTree();
    initArcSuffixTree(newTree, src.at(0).toLatin1() - start.toLatin1(), 0, srcL - 1, nullptr, 0);
    int indexSuffix, indexArc;
    for (int i=1; i<srcL-1; i++)
    {
        QString localSubstr = src.mid(i);
        Arc *newArc = findArcInSuffixTree(src, localSubstr, srcL-i, newTree, indexSuffix, indexArc);
        Node *sourceNode;
        if (!newArc)
            sourceNode = newTree;
        else
            if (indexArc <= newArc->indexFinish)
            {
                sourceNode = new Node();
                Arc *newSplitArc = initArcSuffixTree(sourceNode, src.at(indexArc).toLatin1() - start.toLatin1(), indexArc, newArc->indexFinish, newArc->vertex, newArc->indexVertex);
                newArc->vertex = sourceNode;
                newArc->indexVertex = -1;
                newArc->indexFinish = indexArc - 1;
            }
            else
                sourceNode = newArc->vertex;
        initArcSuffixTree(sourceNode, src.at(i + indexSuffix).toLatin1() - start.toLatin1(), i + indexSuffix, srcL - 1, nullptr, i);
    }
    return newTree;
}

void StringAlgorithms::GetLeavesSuffixTree(Arc *startArc, int numberSymbols, QString &resultIndexes)
{
    if (startArc->indexVertex >= 0)
    {
        resultIndexes += "Найдено вхождение в позиции : ";
        resultIndexes += QString::number(startArc->indexVertex);
        resultIndexes += "\n";
        qDebug() << "Найдено вхождение в позиции : " << QString::number(startArc->indexVertex);
    }
    else
    {
        Node *startNode = startArc->vertex;
        for (int i=0; i<numberSymbols; i++)
        {
            Arc *localArc = startNode->arcs[i];
            if (localArc)
                GetLeavesSuffixTree(localArc, numberSymbols, resultIndexes);
        }
    }
}

void StringAlgorithms::on_naiveAlgorithmPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text(), subString = ui->substrLineEdit->text();
    int srcL = srcString.size(), substrL = subString.size();
    if (srcL >= substrL)
        NaiveStringMatch(srcString, subString);
    else
        if (srcL < substrL)
            QMessageBox::critical(this, "Error", "Исходная строка короче подстроки", QMessageBox::Ok);
        else
            if (srcL == 0 || substrL == 0)
                QMessageBox::critical(this, "Error", "Исходная или искомая строки пустые", QMessageBox::Ok);
}

int StringAlgorithms::max(int a, int b)
{
    return (a > b) ? a : b;
}

int StringAlgorithms::StrSameLen(QString src, int len, int index1, int index2)
{
    int result = 0;
    while (index1 < len && index2 < len && src[index1++] == src[index2++])
        result++;
    return result;
}

int StringAlgorithms::StrSameLenBackward(QString src, int index1, int index2)
{
    int result = 0;
    while (index1 >= 0 && index2 >= 0 && src[index1--] == src[index2--])
        result++;
    return result;
}

int StringAlgorithms::CheckSequence(int srcLine[], int n, int position1, int position2)
{
    int localSeqLength = 0;
    while (position1 < n && srcLine[position1++] == position2)
        localSeqLength++;
    return localSeqLength;
}

void StringAlgorithms::on_PrefixesPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text();
    int srcL = srcString.size();
    if (srcL == 0)
        QMessageBox::critical(this, "Error", "Исходная строка пустая", QMessageBox::Ok);
    else
    {
        int prefixes[srcL];
        PrefixBorderArray(srcString, prefixes);
        QString result = "";
        for (int i=0; i<srcL; i++)
        {
            result += QString::number(prefixes[i]);
            result += " ";
        }
        ui->substrLineEdit->setText(result);
    }
}

void StringAlgorithms::on_SuffixesPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text();
    int srcL = srcString.size();
    if (srcL == 0)
        QMessageBox::critical(this, "Error", "Исходная строка пустая", QMessageBox::Ok);
    else
    {
        int suffixes[srcL];
        SuffixBorderArray(srcString, suffixes);
        QString result = "";
        for (int i=0; i<srcL; i++)
        {
            result += QString::number(suffixes[i]);
            result += " ";
        }
        ui->substrLineEdit->setText(result);
    }
}

void StringAlgorithms::on_naiveZBlocksPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text();
    int srcL = srcString.size();
    if (srcL == 0)
        QMessageBox::critical(this, "Error", "Исходная строка пустая", QMessageBox::Ok);
    else
    {
        int zblocksarray[srcL];
        NaiveZBlockValues(srcString, zblocksarray);
        QString result = "";
        for (int i=0; i<srcL; i++)
        {
            result += QString::number(zblocksarray[i]);
            result += " ";
        }
        ui->substrLineEdit->setText(result);
    }
}

void StringAlgorithms::on_prefixZBlocksPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text();
    int srcL = srcString.size();
    if (srcL == 0)
        QMessageBox::critical(this, "Error", "Исходная строка пустая", QMessageBox::Ok);
    else
    {
        int zblocksarray[srcL];
        PrefixZBlockValues(srcString, zblocksarray);
        QString result = "";
        for (int i=0; i<srcL; i++)
        {
            result += QString::number(zblocksarray[i]);
            result += " ";
        }
        ui->substrLineEdit->setText(result);
    }
}

void StringAlgorithms::on_suffixZBlocksPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text();
    int srcL = srcString.size();
    if (srcL == 0)
        QMessageBox::critical(this, "Error", "Исходная строка пустая", QMessageBox::Ok);
    else
    {
        int zblocksarray[srcL];
        SuffixZBlockValues(srcString, zblocksarray);
        QString result = "";
        for (int i=0; i<srcL; i++)
        {
            result += QString::number(zblocksarray[i]);
            result += " ";
        }
        ui->substrLineEdit->setText(result);
    }
}

void StringAlgorithms::on_kmpAlgorithmPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text(), subString = ui->substrLineEdit->text();
    int srcL = srcString.size(), substrL = subString.size();
    if (srcL >= substrL)
        KnutMorrisPratt(srcString, subString);
    else
        if (srcL < substrL)
            QMessageBox::critical(this, "Error", "Исходная строка короче подстроки", QMessageBox::Ok);
        else
            if (srcL == 0 || substrL == 0)
                QMessageBox::critical(this, "Error", "Исходная или искомая строки пустые", QMessageBox::Ok);
}

void StringAlgorithms::on_bmAlgorithmPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text(), subString = ui->substrLineEdit->text();
    int srcL = srcString.size(), substrL = subString.size();
    if (srcL >= substrL)
        BoierMur(srcString, subString);
    else
        if (srcL < substrL)
            QMessageBox::critical(this, "Error", "Исходная строка короче подстроки", QMessageBox::Ok);
        else
            if (srcL == 0 || substrL == 0)
                QMessageBox::critical(this, "Error", "Исходная или искомая строки пустые", QMessageBox::Ok);
}


void StringAlgorithms::on_bm1AlgorithmPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text(), subString = ui->substrLineEdit->text();
    int srcL = srcString.size(), substrL = subString.size();
    if (srcL >= substrL)
        BoierMur(srcString, subString, true);
    else
        if (srcL < substrL)
            QMessageBox::critical(this, "Error", "Исходная строка короче подстроки", QMessageBox::Ok);
        else
            if (srcL == 0 || substrL == 0)
                QMessageBox::critical(this, "Error", "Исходная или искомая строки пустые", QMessageBox::Ok);
}

void StringAlgorithms::on_krAlgorithmPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text(), subString = ui->substrLineEdit->text();
    int srcL = srcString.size(), substrL = subString.size();
    if (srcL >= substrL)
        KarpRabin(srcString, subString, 11);
    else
        if (srcL < substrL)
            QMessageBox::critical(this, "Error", "Исходная строка короче подстроки", QMessageBox::Ok);
        else
            if (srcL == 0 || substrL == 0)
                QMessageBox::critical(this, "Error", "Исходная или искомая строки пустые", QMessageBox::Ok);
}

void StringAlgorithms::on_saAlgorithmPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text(), subString = ui->substrLineEdit->text();
    int srcL = srcString.size(), substrL = subString.size();
    if (srcL >= substrL)
        ShiftAnd(srcString, subString);
    else
        if (srcL < substrL)
            QMessageBox::critical(this, "Error", "Исходная строка короче подстроки", QMessageBox::Ok);
        else
            if (srcL == 0 || substrL == 0)
                QMessageBox::critical(this, "Error", "Исходная или искомая строки пустые", QMessageBox::Ok);
}

void StringAlgorithms::on_safAlgorithmPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text(), subString = ui->substrLineEdit->text();
    int srcL = srcString.size(), substrL = subString.size();
    if (srcL >= substrL)
        ShiftAndFz(srcString, subString, 2);
    else
        if (srcL < substrL)
            QMessageBox::critical(this, "Error", "Исходная строка короче подстроки", QMessageBox::Ok);
        else
            if (srcL == 0 || substrL == 0)
                QMessageBox::critical(this, "Error", "Исходная или искомая строки пустые", QMessageBox::Ok);
}

void StringAlgorithms::on_stAlgorithmPushButton_clicked()
{
    QString srcString = ui->srcLineEdit->text(), subString = ui->substrLineEdit->text();
    int srcL = srcString.size(), substrL = subString.size();
    if (srcL >= substrL)
    {
        QString resultIndexes = "";
        int indS, indA;
        Node *tree = initSuffixTree(srcString);
        Arc *res = findArcInSuffixTree(srcString, subString, substrL, tree, indS, indA);
        int num = 26;
        GetLeavesSuffixTree(res, num, resultIndexes);
        if (resultIndexes != "")
            QMessageBox::information(this, "Result", resultIndexes, QMessageBox::Ok);
        else
            QMessageBox::critical(this, "Result", "Вхождений не найдено", QMessageBox::Ok);
    }
    else
        if (srcL < substrL)
            QMessageBox::critical(this, "Error", "Исходная строка короче подстроки", QMessageBox::Ok);
        else
            if (srcL == 0 || substrL == 0)
                QMessageBox::critical(this, "Error", "Исходная или искомая строки пустые", QMessageBox::Ok);
}
