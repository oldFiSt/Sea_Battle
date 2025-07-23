#include "../headers/guidewindow.h"
#include "ui_guidewindow.h"

GuideWindow::GuideWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GuideWindow)
{
    ui->setupUi(this);
    setFixedSize(826, 619); // Задаем удобный размер для чтения
    connect(ui->btnClose, &QPushButton::clicked, this, &GuideWindow::accept);

    // Собираем HTML-строку "по частям", чтобы избежать проблем с кодировкой и компилятором
    QString guideText;
    guideText += "<html><head><style>";
    // Добавим немного стилей для раздела создателей
    guideText += R"(
        .creator-section {
            margin-top: 30px;
            padding-top: 20px;
            border-top: 2px solid #d4af37;
        }
        .creator-card {
            /* ИЗМЕНЕНИЕ: Центрируем весь контент внутри карточки */
            text-align: center;
            margin-bottom: 25px;
            background-color: rgba(30, 61, 107, 0.5);
            padding: 15px;
            border-radius: 10px;
        }
        .creator-photo {
            /* ИЗМЕНЕНИЕ: Уменьшаем размер и меняем отступы */
            width: 70px;
            height: 70px;
            border-radius: 50%; /* Делаем фото круглым */
            border: 3px solid #d4af37;
            margin-bottom: 15px; /* Отступ снизу, до текста */
            object-fit: cover; /* Сохраняем пропорции фото */
        }
        .creator-info h3 {
            margin: 0 0 5px 0;
            color: #f8f8ff;
            font-size: 18px;
        }
        .creator-info p {
            margin: 0;
            color: #a3c1da;
            font-size: 14px;
        }
    )";
    guideText += "</style></head><body>";
    guideText += "<h1 style='text-align: center; color: #d4af37;'>Руководство пользователя</h1>";

    guideText += "<h2>Основная цель игры</h2>";
    guideText += "<p>Цель игры \"Морской бой\" - потопить все корабли противника раньше, чем он потопит ваши. Флот состоит из 10 кораблей разного размера.</p>";

    guideText += "<h2 style='color: #a3c1da;'>1. Игра против бота (Одиночный режим)</h2>";
    guideText += "<ol>";
    guideText += "<li><b>Начало игры:</b> В главном меню нажмите кнопку <b>\"Игра против бота\"</b>.</li>";
    guideText += "<li><b>Расстановка кораблей:</b><ul>";
    guideText += "<li>Вы попадёте на экран расстановки. Слева находится панель выбора кораблей (1, 2, 3, 4-палубные) и счётчики.</li>";
    guideText += "<li>Нажмите на кнопку с нужным размером корабля, например, <b>\"4-палубный\"</b>.</li>";
    guideText += "<li>Кликайте на клетки на вашем поле, чтобы разместить корабль. Корабль должен быть прямой линией.</li>";
    guideText += "<li>Корабли не могут касаться друг друга ни сторонами, ни углами.</li>";
    guideText += "<li>Расставьте все 10 кораблей: один 4-палубный, два 3-палубных, три 2-палубных и четыре 1-палубных.</li>";
    guideText += "<li>Когда все корабли будут расставлены, нажмите <b>\"Начать бой\"</b>.</li>";
    guideText += "</ul></li>";
    guideText += "<li><b>Процесс боя:</b><ul>";
    guideText += "<li>Вы увидите два поля: ваше слева и поле противника (бота) справа.</li>";
    guideText += "<li>Если горит надпись <b>\"(Ваш ход)\"</b>, кликайте на клетку на поле противника, чтобы совершить выстрел.</li>";
    guideText += "<li><span style='color: red; font-weight: bold;'>Красная клетка</span> - попадание по кораблю. Вы продолжаете ходить.</li>";
    guideText += "<li><span style='color: white; font-weight: bold;'>Белая клетка</span> - промах. Ход переходит к боту.</li>";
    guideText += "<li>Когда корабль полностью уничтожен, он отмечается красным, а область вокруг него - белыми клетками, так как там не может быть других кораблей.</li>";
    guideText += "</ul></li></ol>";

    guideText += "<h2 style='color: #a3c1da;'>2. Игра с другом (Сетевой режим)</h2>";
    guideText += "<p>Один игрок должен создать игру (<b>Хост</b>), а другой - присоединиться к ней.</p>";
    guideText += "<h4>Для Хоста (создающего игру):</h4>";
    guideText += "<ol>";
    guideText += "<li>В главном меню выберите <b>\"Игра с другом\"</b>, затем <b>\"Создать игру (Хост)\"</b>.</li>";
    guideText += "<li>Введите номер порта (можно оставить стандартный) и нажмите <b>\"Создать\"</b>.</li>";
    guideText += "<li>Откроется окно лобби. Сообщите ваш <b>IP-адрес и порт</b> второму игроку. Ваш IP-адрес для локальной сети будет показан в окне.</li>";
    guideText += "<li>Ожидайте подключения второго игрока.</li></ol>";

    guideText += "<h4>Для Клиента (присоединяющегося к игре):</h4>";
    guideText += "<ol>";
    guideText += "<li>В главном меню выберите <b>\"Игра с другом\"</b>, затем <b>\"Присоединиться к игре\"</b>.</li>";
    guideText += "<li>Введите <b>IP-адрес и порт</b>, который вам сообщил Хост.</li>";
    guideText += "<li>Нажмите <b>\"Подключиться\"</b>.</li></ol>";

    guideText += "<h4>Общий процесс после подключения:</h4>";
    guideText += "<ol>";
    guideText += "<li>Оба игрока попадают на экран расстановки кораблей.</li>";
    guideText += "<li>Каждый расставляет свои корабли и нажимает <b>\"Начать бой\"</b>.</li>";
    guideText += "<li>Появится надпись \"Ожидание второго игрока...\".</li>";
    guideText += "<li>Когда оба игрока будут готовы, игра начнётся автоматически. Право первого хода определяется сервером.</li></ol>";

    // ИЗМЕНЕНИЕ: Надпись перенесена сюда
    guideText += "<p style='text-align: center; font-size: 16px; margin-top: 20px;'><b>Удачи в морских сражениях!</b></p>";

    // --- РАЗДЕЛ "СОЗДАТЕЛИ" ---
    guideText += "<div class='creator-section'>";
    guideText += "<h2 style='text-align: center; color: #d4af37;'>Создатели игры</h2>";

    // Карточка первого создателя
    guideText += "<div class='creator-card'>";
    // Фото будет над текстом
    guideText += "  <img src='qrc:/creators/photo1' class='creator-photo'>";
    guideText += "  <div class='creator-info'>";
    guideText += "    <h3>Козленко Сергей Кириллович</h3>";
    guideText += "    <p>Студент Инженерной школы информационных технологий и робототехники Томского политехнического университета, обучается в группе 8В32 по направлению подготовки 09.03.01 – Информатика и вычислительная техника. Выпускник МБОУ «Лицей №17» города Славгорода Алтайского края.</p>";
    guideText += "  </div>";
    guideText += "</div>";

    // Карточка второго создателя
    guideText += "<div class='creator-card'>";
    // Фото будет над текстом
    guideText += "  <img src='qrc:/creators/photo2' class='creator-photo'>";
    guideText += "  <div class='creator-info'>";
    guideText += "    <h3>Рычко Егор Сергеевич</h3>";
    guideText += "    <p>Студент Инженерной школы информационных технологий и робототехники Томского политехнического университета, обучается в группе 8В32 по направлению подготовки 09.03.01 – Информатика и вычислительная техника. Выпускник МБОУ «Лицей №17» города Славгорода Алтайского края.</p>";
    guideText += "  </div>";
    guideText += "</div>";

    guideText += "</div>";
    // --- КОНЕЦ РАЗДЕЛА ---

    guideText += "</body></html>";

    ui->textBrowser->setHtml(guideText);
}

GuideWindow::~GuideWindow()
{
    delete ui;
}
