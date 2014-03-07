#ifndef HTML_H
#define HTML_H
#include <QStringList>
#include <QString>

#define MARKER_CONTENT( content )  "<div class='pwcontent'>" content "</div>"
#define TITLE( content ) "<h2>" content "<h2>"
#define IMAGE( url, altname) "<img src='" url "' alt='" altname "'/>"
#define BODY( content ) content

class html
{
	QStringList _lines;
public:
	html(){}

	html& title( const QString& content) {
		_lines.append( "<h2>");
		_lines.append(content);
		_lines.append( "</h2>");
		return *this;
	}
	html& body( const QString& content) {
		_lines.append( "<p>");
		_lines.append( content);
		_lines.append( "</p>");
		return *this;
	}
	html& image( const QString& url, const QString& altname) {
		_lines.append(QString( IMAGE("%1","%2")).arg(url).arg(altname)); return *this;
	}
	QString get() { return QString( MARKER_CONTENT( "%1" ) ).arg(_lines.join("")); }

	operator QString() {
		return get();
	}
};

#endif // HTML_H
