#pragma once

class ChatScreen
{
public:
	bool Show();

	ChatScreen(msh::ViewModel& view_model, msh::Presenter& presenter);

private:
	msh::ViewModel& view_model_;
	msh::Presenter& presenter_;
};