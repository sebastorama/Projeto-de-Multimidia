class Dc
  def self.do_dc(vector, count)
    (count-1).downto(1) do |i|
      vector[i] -= vector[i-1]
    end
    vector
  end

  def self.undo_dc(vector, count)
    (1).upto(count-1) do |i|
      vector[i] += vector[i-1]
    end
    vector
  end
end


describe Dc do
  describe "#do_dc" do
    it "does difference encoding" do
      Dc.do_dc([12, 11, 11, 15], 4).should == [12, -1, 0, 4]
    end
  end

  describe "#undo_dc" do
    it "does difference encoding" do
      Dc.undo_dc([12, -1, 0, 4], 4).should == [12, 11, 11, 15]
    end
  end
end
