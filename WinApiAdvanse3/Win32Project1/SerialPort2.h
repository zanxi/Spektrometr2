#include ".\stdwin.h"

/*
ZeroMemory (&dcb, sizeof (DCB));// не знает что ето такое
*/
//объявим структуру для асинхронной работы порта
OVERLAPPED over;
//для выделения сигнала
DWORD dwSignal;
//объявим структуру для конфигурации СОМ порта
DCB dcb;
//дескриптор порта
HANDLE hPortDat = NULL;
//переменная для чтения
char Data;
//==================================
//---------------------------------------------------------------------------
class MuClDataOutPort
{
private:

    //пишим функцию инициализации порта
    bool InnitPort()
    {
        //открываем порт СОМ 1
        hPortDat = CreateFile(L"COM1", GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED, NULL);
        if (hPortDat == INVALID_HANDLE_VALUE)//если порт не удалось открыть
        {
            CloseHandle(hPortDat);
            return false;//выходим из функции с ошибкой
        }
        //настраиваем параметры порта
        dcb.BaudRate = CBR_19200;//скорость передачи
        dcb.ByteSize = 8;//размер передачи
        dcb.StopBits = ONESTOPBIT;//один стоповый бит
        dcb.Parity = NULL;
        //проверяем на правильность настройки

        if (!SetCommState(hPortDat, &dcb))  //тоже не работает....
        {
            CloseHandle(hPortDat);
            return false;//выходим из функции с ошибкой
        }

        //если все выполнилось то возвращаем положительный результат
        return true;
    }
    //----------------------------------------------------
    //функция чтения одного байта данных
    BYTE ReadByteCOM()
    {
        if (InnitPort())
        {
            BYTE read = 0;
            DWORD dwByteRead = 0;
            do
            {//читаем байт из порта
                if (!ReadFile(hPortDat, &read, sizeof(BYTE), &dwByteRead, NULL))
                {
                    return 0xFF;
                }
            } while (!dwByteRead);
            return read;//возвращаем данные
        }
        else
        {
            return 0xFF;
        }
    }
    //------------------------------------------------------
    //функция чтения одного байта данных
    bool WriteByteCOM(char bufer)
    {
        if (InnitPort())
        {
            DWORD dwByteWrite = 0;
            if (!WriteFile(hPortDat, &bufer, sizeof(char), &dwByteWrite, NULL))
            {
                return false;
            }
            return true;//возвращаем сведение о выполнении
        }
        else
        {
            return false;
        }
    }
    //=============================================
    //функция чтения массива данных
    //=============================================
    //=============================================
    //=============================================
public:
    //общая функция для работы с портом
    void GeneralCOMRead()
    {
        //проверяем сигнал в линии
        if (dwSignal & EV_DSR)//данные готовы для чтения
        {
            //читаем байт из порта
            Data = ReadByteCOM();
            //сохраняем байт куда-либо
        }
    }
    //---------------------------------------------------------
    void GeneralCOMWrite(char InData)
    {
        //проверяем сигнал в линии
        if (dwSignal & EV_CTS)//данные готовы для записи
        {
            //пердаем байт из вне и записываем его в порт
            WriteByteCOM(InData);
        }
    }
    //---------------------------------------------------------
    void CloseCOM()
    {
        if (over.hEvent)
        {
            CloseHandle(over.hEvent);//закрываем объект событие
        }
        if (hPortDat)
        {
            CloseHandle(hPortDat);
            hPortDat = NULL;
        }
    }
};
#pragma package(smart_init)
