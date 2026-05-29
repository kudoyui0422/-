#pragma once

class Player 
{
#pragma once
	// ‰æ‘œ‚ğ“Ç‚İ‚Ş‚½‚ß‚ÌŠÖ”‚ğuéŒ¾v‚·‚é
	void Image_Load();
public:
	Player();
	~Player();
	void Update() ;
	void Draw();

private:
	int hImage;
	float x, y;
	float velocity;
	bool onGround;
	Field* field;

};

