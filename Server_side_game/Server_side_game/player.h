class player {
private:
	int xpos;
	int ypos;
public:
	player() = default;
	player(int xpos_, int ypos_);
	~player() = default;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void setCoordinates(int xpos_, int ypos_);

	int get_xpos()const;
	int get_ypos()const;

	bool operator==(const player& p2);
};