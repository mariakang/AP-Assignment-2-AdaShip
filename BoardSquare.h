#pragma once

using namespace std;

class BoardSquare {
  private:
    int boatId_;
    bool torpedoed_;
    bool hasMine_;
    int maxBoatLengthVertical_;
    int maxBoatLengthHorizontal_;

  public:
    BoardSquare() {
      boatId_ = -1;
      torpedoed_ = false;
      hasMine_ = false;
      maxBoatLengthVertical_ = 0;
      maxBoatLengthHorizontal_ = 0;
    }
    int boatId() {
      return boatId_;
    }
    void setBoatId(int boatId) {
      boatId_ = boatId;
    }
    bool torpedoed() {
      return torpedoed_;
    }
    void setTorpedoed(bool torpedoed) {
      torpedoed_ = torpedoed;
    }
    bool hasMine() {
      return hasMine_;
    }
    void setHasMine(bool hasMine) {
      hasMine_ = hasMine;
    }
    int maxBoatLengthVertical() {
      return maxBoatLengthVertical_;
    }
    void setMaxBoatLengthVertical(int maxBoatLengthVertical) {
      maxBoatLengthVertical_ = maxBoatLengthVertical;
    }
    int maxBoatLengthHorizontal() {
      return maxBoatLengthHorizontal_;
    }
    void setMaxBoatLengthHorizontal(int maxBoatLengthHorizontal) {
      maxBoatLengthHorizontal_ = maxBoatLengthHorizontal;
    }
    BoardSquare& operator=(const BoardSquare& rhs) {
      if (this != &rhs) {
        boatId_ = rhs.boatId_;
        torpedoed_ = rhs.torpedoed_;
        hasMine_ = rhs.hasMine_;
        maxBoatLengthVertical_ = rhs.maxBoatLengthVertical_;
        maxBoatLengthHorizontal_ = rhs.maxBoatLengthHorizontal_;
      }
      return (*this);
    }
};