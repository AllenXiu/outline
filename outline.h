#ifndef OUTLINE_H
#define OUTLINE_H

#include "cocos2d.h"
#include <map>

using namespace cocos2d;
using namespace std;

struct Outline;
struct Creator;

//���node���������������Ϊnode�ĳ���ģ��
struct Outline {
	//������Ϣ��outlineĬ��ֻ�ܱ��node��label��sprite����
	bool isLabel = false, isSprite = false;
	int label_fontSize;
	string label_string, imageFile;

	//���һ��ʹ�ô�outline�����node
	Node *lastNode;

	//ģ����Ϣ
	string name;
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;
	float anchorX = 0.5f;
	float anchorY = 0.5f;
	float scaleX = 1;
	float scaleY = 1;
	float rotation = 0;
	int opacity = 255;
	bool visible = true;
	int zOrder = 0;
	int colorR = 255;
	int colorG = 255;
	int colorB = 255;

	//ָ���Ӧ��creatorʵ��
	Creator *pOit;

	//�ӽڵ��outline
	vector<Outline*> children;

	//��node�Ĵ��캯��
	std::function<Node*(Creator*, Node*)> createNode;

	//��"o__"Ϊǰ׺���Զ��������Ϣ����ڸó�Ա
	string mapAble; 

	//����node��������node
	Node *create(Node *parent) {
		auto node = lastNode = createNode(pOit, parent);
		node->setName(name);
		reset(node);
		for (int i = 0; i < children.size(); i++) {
			children[i]->create(node);
		}
		return node;
	}

	//Ϊ�����node����ģ�������
	void reset(Node *node){
		node->setPositionX(x);
		node->setPositionY(y);
		if (width>0 && height>0)
			node->setContentSize(Size(width, height));
		node->setAnchorPoint(Vec2(anchorX, anchorY));
		node->setScaleX(scaleX);
		node->setScaleY(scaleY);
		node->setRotation(rotation);
		node->setOpacity(opacity);
		node->setVisible(visible);
		node->setLocalZOrder(zOrder);
		node->setColor(Color3B(colorR, colorG, colorB));
	}
};

//���нڵ�ģ��ṹ��Ļ��ṹ��
struct Creator{
	Outline *outline;
	Node *create(Node *parent){
		return outline->create(parent);
	}
	Node *lastNode() {
		return outline->lastNode;
	}
};

//Ĭ�ϴ���node�ĺ���
static auto createNode = [](Creator *creator, Node *parent)->Node* {
	Node *node;
	if (creator) {
		if (creator->outline->isSprite){
			node = Sprite::create(creator->outline->imageFile);
		}
		else if (creator->outline->isLabel){
			int fontSize;
			auto label = Label::create();
			label->setString(creator->outline->label_string);
			label->setSystemFontSize(creator->outline->label_fontSize);
			node = label;
		}
		else
			node = Node::create();
	}
	else
		node = Node::create();
	if (parent)
		parent->addChild(node);
	return node;
};

//���ж����ṹ��Ļ��ṹ��
struct AnimBase {
protected:
	//������floatתint�ضϲ�����ƫ��
	map<Node*,float> offsets;
	void addOpacity(Node* node, float addOpacity){
		addOpacity += offsets[node];
		int trueAdd = (int)addOpacity;
		offsets[node] = addOpacity - trueAdd;

		int opacity = node->getOpacity();
		int newOpacity = opacity + trueAdd;
		if (newOpacity > 255)
			newOpacity = 255;
		else if (newOpacity < 0)
			newOpacity = 0;
		node->setOpacity(newOpacity);
	}


	Node *getChild(Node *parent,string path){
		Node *child = NULL;
		vector<string> names;
		string name = path;
		int index = name.find("/");
		while (name != ""&&index != 0) {
			if (index > 0) {
				names.push_back(name.substr(0, index));
				name = name.substr(index + 1);
				index = name.find("/");
			}
			else {
				names.push_back(name);
				break;
			}
		}
		if (names.size() > 0) {
			child = parent;
			for (int i = 0; i < names.size(); i++) {
				if (child)
					child = child->getChildByName(names[i]);
			}
		}
		return child;
	}
public:
	//���ŵ���֡��
	int frameIndex;
	//��Ӧschedule��key
	std::string key;
	//��Ҫ���ŵ�node
	Node *node;
	bool played;
	//�Ƿ�ѭ��
	bool loop;
	//�������ص�
	std::function<void(string)> callback;
	void pause() {
		node->unschedule(key);
	}

	void stop() {
		node->unschedule(key);
		delete this;
	}

	virtual void play(Node *pNode, const std::string &key,bool loop) {
		this->node = pNode;
		this->key = key;
		this->frameIndex = 0;
		this->loop = loop;

		if (pNode) {
			played = true;
			resume();
		}
	}
	void play(Node *pNode, const std::string &key,bool loop, std::function<void(string)> callback) {
		if (pNode) {
			this->callback = callback;
		}
		play(pNode, key, loop);
	}
	virtual void resume() = 0;
};

#endif // !OUTLINE_H
