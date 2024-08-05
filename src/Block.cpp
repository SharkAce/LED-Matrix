#include <Block.hpp>

Block::Block(Block::Config config) : font(config.font) {
	textProvider = std::move(config.textProvider);
	color = config.color;
	interval = config.interval;
	x = config.x;
	y = config.y;
}

std::vector<Block> Block::createBlocksFromJson(const std::string& filename, rgb_matrix::Font& font) {
	std::ifstream file(filename);
	nlohmann::json j;
	file >> j;

	std::vector<Block> blocks;

	for (const auto& item : j.at("blocks")) {
		auto textProvider = TextProvider::createFromJSON(item);

		int r = item.at("color")[0].get<int>();
		int g = item.at("color")[1].get<int>();
		int b = item.at("color")[2].get<int>();
		rgb_matrix::Color color(r, g, b);

		int x = item.at("position").at("x").get<int>();
		int y = item.at("position").at("y").get<int>();

		int interval = item.at("interval").get<int>();

		Block::Config config = {
			.textProvider = std::move(textProvider),
			.font = font,
			.color = color,
			.interval = interval,
			.x = x,
			.y = y
		};

		blocks.emplace_back(std::move(config));
	}

	return blocks;
}

void Block::draw(rgb_matrix::RGBMatrix* canvas) {
	if (textProvider) {
		std::string text = textProvider->getText();
		rgb_matrix::DrawText(canvas, font, x, y, color, text.c_str());
	}
}

void Block::update() {
	auto now = std::chrono::steady_clock::now();
	if (textProvider && now - last_update_time >= (std::chrono::seconds)interval) {
		textProvider->update();
		last_update_time = now;
	}
}
