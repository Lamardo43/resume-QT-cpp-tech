Здравствуйте! Это тестовое задание.

ТЗ:

Client-Server application for monitoring work activity

Explanation
Simple application to show current work activity of all employers in organisation

Example applications 
https://www.teramind.co/solutions/employee-monitoring-software
https://veriato.com/product/

Client - c/c++
Silent launches on user logon and work in background
Communicates with server at any protocol 

Server - desktop or web interface - any language 
List all connected clients - domain/machine/ip/user
Show client’s last active time
Ability to get screenshot from client’s desktop 


При запуске программы она сворачивается в трей (Чтобы открыть окно, нужно нажать пкм по значку и "Восстановить") и связывается с сервером и запрашивает историю подключений к серверу. При выборе конкретного ip адреса из списка, справа отображается последнее действие этого адреса, а так же возможность получить скриншот с рабочего стола.

Автозапуска приложения нет, тк оно запускается через QT Creator. 

Для корректной работы необходимо в файл clinet - singletonClient.cpp вставить ip адрес сервера (5 строчка) 
