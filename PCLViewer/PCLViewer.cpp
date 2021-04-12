#include "PCLViewer.h"
#include "ui_PCLViewer.h"

PCLViewer::PCLViewer(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::PCLViewer)
{
	ui->setupUi(this);

	//��l��
	init();
}

// ���o����I����󧤼жb���̵u�Z��
double getMinValue(PointT p1, PointT p2);
// ���o����I�����y�жb���̪��Z��
double getMaxValue(PointT p1, PointT p2);




void PCLViewer::init()
{
	// �I����l��
	m_currentCloud.reset(new PointCloudT);
	// �i���ƪ�l��
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	// �]�mVTK�i���Ƶ��f���w
	ui->qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
	// �]�m��������
	viewer->setupInteractor(ui->qvtkWidget->GetInteractor(), ui->qvtkWidget->GetRenderWindow());
	// �K�[���жb
	viewer->addCoordinateSystem(1.0);

	QStringList headerList;
	headerList.append(tr("���W"));
	headerList.append(tr("���j�p"));
	headerList.append(tr("�����|"));
	ui->treeWidget->setHeaderLabels(headerList);
	ui->treeWidget->setColumnWidth(1, 150);
	// �ƥ�
	connect(&heightRampDlg, SIGNAL(setHeightRamp(int, double)), this, SLOT(setHeightRamp(int, double)));
}

void PCLViewer::on_action_open_triggered()
{
	// ���o�I�����|
	QString fileName = QFileDialog::getOpenFileName(this, "����I�����", ".//", "�I�����(*.pcd);;�Ҧ����(*.*)");

	if (!fileName.isEmpty())
	{
		//�M���I��
		m_currentCloud->clear();
		viewer->removeAllPointClouds();
		viewer->removeAllCoordinateSystems();

		// Ū���I���ƾ�
		pcl::io::loadPCDFile(fileName.toStdString(), *m_currentCloud);
	}
	//�K�[�����
	viewer->addPointCloud(m_currentCloud);

	pcl::getMinMax3D(*m_currentCloud, p_min, p_max);

	double scale = getMinValue(p_max, p_min);
	maxLen = getMaxValue(p_max, p_min);

	viewer->addCoordinateSystem(scale * 0.3);

	// ���]����
	viewer->resetCamera();

	//��s���f
	ui->qvtkWidget->update();
}

// ���o�v��
void PCLViewer::on_action_getimage_triggered() {
	imu_renderer::get_camara_image;
}



// Ū����e��Ƨ����ɮ�
void PCLViewer::on_pushButton_clicked() {
	QDir dir("D:\123\RGB"); //���|
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Time);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QTreeWidgetItem* newItem = new QTreeWidgetItem();
		newItem->setText(0, fileInfo.fileName());
		newItem->setText(1, QString::number(fileInfo.size()));
		newItem->setText(2, fileInfo.absoluteFilePath());
		ui->treeWidget->addTopLevelItem(newItem);
	}
}



//���]����
void PCLViewer::on_action_reset_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->resetCamera();
		ui->qvtkWidget->update();
	}
}

// ������
void PCLViewer::on_action_up_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0, 0, 1, 0);
		ui->qvtkWidget->update();
	}
}

// �e����
void PCLViewer::on_action_front_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), p_min.y - 2 * maxLen, 0.5 * (p_min.z + p_max.z), 0.5 * (p_min.x + p_max.x), 0, 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui->qvtkWidget->update();
	}
}

//������
void PCLViewer::on_action_left_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_min.x - 2 * maxLen, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui->qvtkWidget->update();
	}
}

//�����
void PCLViewer::on_action_back_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), p_max.y + 2 * maxLen, 0.5 * (p_min.z + p_max.z), 0.5 * (p_min.x + p_max.x), 0, 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui->qvtkWidget->update();
	}
}

// �k����
void PCLViewer::on_action_right_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_max.x + 2 * maxLen, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui->qvtkWidget->update();
	}
}

// ������
void PCLViewer::on_action_bottom_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), p_min.z - 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0, 0, 1, 0);
		ui->qvtkWidget->update();
	}
}

// �e�b��
void PCLViewer::on_action_frontIso_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_min.x - 2 * maxLen, p_min.y - 2 * maxLen, p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 1, 1, 0);
		ui->qvtkWidget->update();
	}
}

// ��b��
void PCLViewer::on_action_backIso_triggered()
{
	viewer->setCameraPosition(p_max.x + 2 * maxLen, p_max.y + 2 * maxLen, p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), -1, -1, 0);
	ui->qvtkWidget->update();
}

// �]�m�I���C��
void PCLViewer::on_action_setColor_triggered()
{
	QColor color = QColorDialog::getColor(Qt::white, this, "�]�m�I���C��", QColorDialog::ShowAlphaChannel);

	viewer->removeAllPointClouds();
	pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(m_currentCloud, color.red(), color.green(), color.blue());
	viewer->addPointCloud(m_currentCloud, singelColor, "myCloud", 0);
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, color.alpha() * 1.0 / 255, "myCloud");

	ui->qvtkWidget->update();

}

// �]�m���״�V
void PCLViewer::on_action_heightRamp_triggered()
{
	heightRampDlg.show();
}

// �i�氪�״�V
void PCLViewer::setHeightRamp(int dir, double height1)
{
	//�M���I��
	viewer->removeAllPointClouds();
	m_heightCloudList.clear();

	double min_range = 0;
	double max_range = 0;
	std::string field = "x";

	switch (dir)
	{
	case 0:
		min_range = p_min.x;
		max_range = p_max.x;
		field = "x";
		break;

	case 1:
		min_range = p_min.y;
		max_range = p_max.y;
		field = "y";
		break;

	case 2:
		min_range = p_min.z;
		max_range = p_max.z;
		field = "z";
		break;
	default:
		break;
	}

	for (double i = min_range - 1; i < max_range + height;)
	{
		PointCloudT::Ptr cloudTemp(new PointCloudT());

		pcl::PassThrough<PointT> pass;			//���q�o�i����H
		pass.setInputCloud(m_currentCloud);		//��J�I��
		pass.setFilterFieldName(field);			//�]�m�L�o�r�q
		pass.setFilterLimits(i, i + height1);	//�]�m�L�o�d��
		pass.setFilterLimitsNegative(false);	//�]�m�O�d�r�q
		pass.filter(*cloudTemp);				//�����o�i

		i += height1;

		m_heightCloudList.append(cloudTemp);
	}

	//���q��V
	for (int j = 0; j < m_heightCloudList.size(); j++)
	{
		pcl::visualization::PointCloudColorHandlerGenericField<PointT> fieldColor(m_heightCloudList.at(j), field);
		std::string index = std::to_string(j);
		viewer->addPointCloud(m_heightCloudList.at(j), fieldColor, index);
	}

}

double getMinValue(PointT p1, PointT p2)
{
	double min = 0;

	if (p1.x - p2.x > p1.y - p2.y)
	{
		min = p1.y - p2.y;
	}
	else
	{
		min = p1.x - p2.x;
	}

	if (min > p1.z - p2.z)
	{
		min = p1.z - p2.z;
	}

	return min;
}


double getMaxValue(PointT p1, PointT p2)
{
	double max = 0;

	if (p1.x - p2.x > p1.y - p2.y)
	{
		max = p1.x - p2.x;

	}
	else
	{
		max = p1.y - p2.y;
	}

	if (max < p1.z - p2.z)
	{
		max = p1.z - p2.z;
	}

	return max;
}

PCLViewer::~PCLViewer()
{
	delete ui;
}