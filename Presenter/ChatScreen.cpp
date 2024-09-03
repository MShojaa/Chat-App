#include "pch.h"
#include "ChatScreen.h"

ChatScreen::ChatScreen(msh::ViewModel& view_model, msh::Presenter& presenter) :
	view_model_(view_model), presenter_(presenter) {
	/* Do nothing */
}

bool ChatScreen::Show() {

	presenter_.Print("\n");

	static auto valid_characters = [](char key) {
		return (
			key != msh::kDoubleKey &&
			key != '*' && key != '|' &&
			//key != '?' && key != '\"' &&
			key != '<' && key != '>'
		);
	};

	std::optional<std::string> result;

	std::thread receive_message_thread([&] {
		std::string received_massage;
		while (true) {
			bool result = view_model_.ReceiveMessage(received_massage);
			if (!result)
				presenter_.log("Couldn't receive message");

			presenter_.ClearLine().Print("Not you: ").log(received_massage).Print("You: ");
		}
	});

	std::thread receive_file_thread([&] {
		while (true) {
			bool result = view_model_.ReceiveFile();
			if (!result)
				presenter_.log("Couldn't receive file");
			else
				presenter_.log("Received successfully");
		}
	});

	msh::Flow<std::string> buffer;
	msh::Flow<bool> is_message;

	std::thread get_input_thread([&] {
		while (true) {
			presenter_.Print("You: ");
			result = presenter_.GetLine(valid_characters);
			if (!result.has_value()) {
				buffer.finished();
				return;
			}

			if (result.value() == "$$File$$") {
				is_message.push_async(false);
				result = presenter_.GetLine(valid_characters);
				if (!result.has_value()) {
					buffer.finished();
					return;
				}
			} else {
				is_message.push_async(true);
			}

			if (result.value() == "$$File$$")
				continue;

			buffer.push_async(result.value());
		}
	});

	std::thread send_file_thread([&] {
		while (true) {
			if (is_message.wait_to_fill() && is_message.first_element() == false) {
				result = buffer.pop_async();
				if (!result.has_value())
					return;

				is_message.clear_all();
				if (view_model_.SendFile(result.value()))
					presenter_.log("File sent successfully!");
				else
					presenter_.log("Couldn't send the file");
			} else {
				/*std::this_thread::sleep_for(std::chrono::seconds(1));*/
				(void)is_message.wait_to_empty();
			}
		}
	});

	std::thread send_message_thread([&] {
		while (true) {
			if (is_message.wait_to_fill() && is_message.first_element() == true) {
				result = buffer.pop_async();
				if (!result.has_value())
					return;

				is_message.clear_all();

				if (!view_model_.SendMessage(result.value()))
					presenter_.log("Couldn't send the message");
			} else {
				(void)is_message.wait_to_empty();
			}
		}
	});

	get_input_thread.join();
	send_message_thread.join();
	send_file_thread.join();

	receive_message_thread.join();
	receive_file_thread.join();

	return false;
}