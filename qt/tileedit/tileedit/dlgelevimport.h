#ifndef DLGELEVIMPORT_H
#define DLGELEVIMPORT_H

#include "elv_io.h"
#include <QDialog>

namespace Ui {
	class DlgElevImport;
}

class tileedit;
class ElevTileBlock;

class DlgElevImport : public QDialog
{
	Q_OBJECT

public:
	DlgElevImport(tileedit *parent);

public slots:
	void onOpenFileDialog();
	void onOpenMetaFileDialog();
	void onMetaFileChanged(const QString&);
	void onSelectAllTiles();
	void onSelectTileRange();
	void onPropagateChanges(int);
	void accept();

protected:
	bool scanMetaFile(const char *fname, ImageMetaInfo &meta);

private:
	Ui::DlgElevImport *ui;
	tileedit *m_tileedit;
	bool m_pathEdited, m_metaEdited;
	int m_propagationLevel;
	ImageMetaInfo m_metaInfo;
};

#endif // !DLGELEVIMPORT_H
