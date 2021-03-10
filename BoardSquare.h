#pragma once

class BoardSquare {
  private:
    /** The boat ID. */
    int boatId_;
    /** Whether or not the square has been torpedoed. */
    bool torpedoed_;
    /** Whether or not the square has a mine. */
    bool hasMine_;
    /** The maximum length of boat which could placed in the square vertically. */
    int maxBoatLengthVertical_;
    /** The maximum length of boat which could placed in the square horizontally. */
    int maxBoatLengthHorizontal_;
    /** The relative probability of a boat being placed in the square. */
    int probabilityDensity_;

  public:
    /** Constructor method. */
    BoardSquare() {
      // a boat's ID corresponds to its index within the fleet (zero-based);
      // having a boatId of -1 means the square is unoccupied 
      boatId_ = -1;
      torpedoed_ = false;
      hasMine_ = false;
      maxBoatLengthVertical_ = 0;
      maxBoatLengthHorizontal_ = 0;
      probabilityDensity_ = 0;
    }

    /** 
     * Returns the ID of the boat occupying the square, or -1 if the square is
     * unoccupied.
     *
     * A boat ID is not an attribute of the Boat object; it corresponds to the
     * boat's index within the fleet (zero-based).
     */
    int boatId() {
      return boatId_;
    }

    /** Sets the boat ID to the given value. */
    void setBoatId(int boatId) {
      boatId_ = boatId;
    }

    /** Returns whether or not the square has been torpedoed. */
    bool torpedoed() {
      return torpedoed_;
    }

    /** Sets whether or not the square has been torpedoed. */
    void setTorpedoed(bool torpedoed) {
      torpedoed_ = torpedoed;
    }

    /** Returns whether or not the square contains a mine. */
    bool hasMine() {
      return hasMine_;
    }

    /** Sets whether or not the square contains a mine. */
    void setHasMine(bool hasMine) {
      hasMine_ = hasMine;
    }

    /** 
     * Returns the maximum length of boat which could be placed in the 
     * square vertically.
     *
     * Being 'placed in the square' means starting in the square,
     * where a boat's 'start' is its top left coordinate.
     */
    int maxBoatLengthVertical() {
      return maxBoatLengthVertical_;
    }

    /** 
     * Sets the maximum length of boat which could be placed in the 
     * square vertically.
     *
     * Being 'placed in the square' means starting in the square,
     * where a boat's 'start' is its top left coordinate.
     */
    void setMaxBoatLengthVertical(int maxBoatLengthVertical) {
      maxBoatLengthVertical_ = maxBoatLengthVertical;
    }

    /** 
     * Returns the maximum length of boat which could be placed in the 
     * square horizontally.
     *
     * Being 'placed in the square' means starting in the square,
     * where a boat's 'start' is its top left coordinate.
     */
    int maxBoatLengthHorizontal() {
      return maxBoatLengthHorizontal_;
    }

    /** 
     * Sets the maximum length of boat which could be placed in the 
     * square horizontally.
     *
     * Being 'placed in the square' means starting in the square,
     * where a boat's 'start' is its top left coordinate.
     */
    void setMaxBoatLengthHorizontal(int maxBoatLengthHorizontal) {
      maxBoatLengthHorizontal_ = maxBoatLengthHorizontal;
    }

    /** 
     * Returns the relative probability that the square is occupied,
     * or 0 if the square has already been torpedoed.
     *
     * This is used in the enhanced targeting algorithm, and therefore
     * only uses information available to the board owner's opponent.
     *
     * Squares which have already been torpedoed will be given a
     * relative probability of 0, so as not to be targeted again. 
     */
    int probabilityDensity() {
      return probabilityDensity_;
    }

    /** Resets the relative probability that the square is occupied to 0. */
    void resetProbability() {
      probabilityDensity_ = 0;
    }

    /** Increases the relative probability by 1. */
    void incrementProbability() {
      probabilityDensity_++;
    }

    /** 
     * Returns whether or not the square has been torpedoed and it 
     * resulted in a miss.
     */
    bool isMiss() {
      return torpedoed_ && boatId_ < 0;
    }

    /** Assignment operator. */
    BoardSquare& operator=(const BoardSquare& rhs) {
      // if the rhs already equals this object, no need to do anything
      if (this != &rhs) {
        // copy over the attributes of rhs
        boatId_ = rhs.boatId_;
        torpedoed_ = rhs.torpedoed_;
        hasMine_ = rhs.hasMine_;
        maxBoatLengthVertical_ = rhs.maxBoatLengthVertical_;
        maxBoatLengthHorizontal_ = rhs.maxBoatLengthHorizontal_;
        probabilityDensity_ = rhs.probabilityDensity_;
      }
      // return the dereferenced pointer
      return (*this);
    }
};