#ifndef MYWEBPAGE_H
#define MYWEBPAGE_H

#include<QWebPage>
class mywebpage : public QWebPage
{
public :
    mywebpage();
    QString userAgentForUrl(const QUrl &url) const;
};

#endif // MYWEBPAGE_H
