#include "mywebpage.h"

#define U_A "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko (bigrats web browser  0.4.7.9r)"

mywebpage::mywebpage():QWebPage()
{
}

QString mywebpage:: userAgentForUrl(const QUrl &url) const
{
   return QString(U_A);
}

