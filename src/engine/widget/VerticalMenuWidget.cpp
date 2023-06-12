#include "VerticalMenuWidget.hpp"

#include <engine/widget/TextBox.hpp>
#include <window/glfw.hpp>

VerticalMenuWidget::VerticalMenuWidget(string title)
  : Title(title), Cursor(0), Scroll(0), ItemHeight(30.0f)
{
  fontNormal = new FTGLPixmapFont("res/font/D2Coding.ttf");
  fontBold = new FTGLPixmapFont("res/font/D2CodingBold.ttf");
}

void VerticalMenuWidget::Draw() {
  BackgroundColor = { 0.1f, 0.1f, 0.1f };
  Widget::Draw();
  BoxCoord innerBox = GetInnerBox();

  int itemCount = data.size();
  int slotCount = (innerBox.height / ItemHeight) - 1;
  int maxSlotCount = itemCount - Scroll;
  float itemPosY = innerBox.y;

  if (maxSlotCount < slotCount) {
    slotCount = maxSlotCount;
  }

  fontBold->FaceSize(50);
  TextBox titleTextBox(Title, fontBold);
  titleTextBox.Box = { innerBox.x, itemPosY, innerBox.width, ItemHeight };
  itemPosY -= ItemHeight;
  titleTextBox.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  titleTextBox.Draw();
  
  for (unsigned int index = Scroll; index < Scroll + slotCount && index < itemCount; ++index) {
    string title = data[index].title;

    fontNormal->FaceSize(50);
    TextBox itemTextBox(title, fontNormal);
    itemTextBox.Box = { innerBox.x, itemPosY, innerBox.width, ItemHeight };
    itemPosY -= ItemHeight;
    itemTextBox.Layout.SetAlignment(FTGL::ALIGN_CENTER);
    itemTextBox.Draw(index == Cursor);
  }
}

unsigned int VerticalMenuWidget::AddMenu(string title, MenuActionFunc action) {
  unsigned int id = data.size();
  data.push_back({ title, action });
  return id;
}

void VerticalMenuWidget::RemoveMenu(unsigned int id) {
  unsigned int size = data.size();

  if (id < size) {
    data.erase(next(data.begin(), id));
  }
}

void VerticalMenuWidget::RemoveMenu(string title) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (it->title == title) {
      data.erase(it);
      return;
    }
  }
}

void VerticalMenuWidget::KeyboardControlSystem() {  
  if (WindowGetKey(GLFW_KEY_W, 100000)) {
    if (Cursor == 0) return;

    --Cursor;
    if (Cursor < Scroll) Scroll = Cursor;

  } else if (WindowGetKey(GLFW_KEY_S, 100000)) {
    BoxCoord innerBox = GetInnerBox();

    int itemCount = data.size();
    if (Cursor + 1 >= itemCount) return;

    int slotCount = (innerBox.height / ItemHeight) - 1;
    int maxSlotCount = itemCount - Scroll;
    float itemPosY = innerBox.y;

    if (maxSlotCount < slotCount) slotCount = maxSlotCount;

    ++Cursor;
    if (Cursor > Scroll + slotCount) Scroll += 2;
    
  } else if (WindowGetKey(GLFW_KEY_SPACE, 150000)) {
    string title = data[Cursor].title;
    data[Cursor].func(title);
  }
}