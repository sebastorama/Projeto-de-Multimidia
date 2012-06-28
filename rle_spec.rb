#encoding: utf-8
class Rle
  def self.next_non_zero(vector, index)
    for i in (index+1)..(vector.size-1)
      return i if vector[i] != 0
    end

    return -1
  end

  def self.do_rle(vector)
    result = []
    current_index = 0
    while(next_non_zero(vector, current_index) != -1) do
      nnz = next_non_zero(vector, current_index)
      result << {skip: nnz - current_index-1, value: vector[nnz] }
      current_index = nnz
    end
    result << {skip:0, value: 0} if(current_index < vector.size-1)
    result
  end

  def self.fill_with_zeros(vector, size,  count)
    while(count > 0)
      count -= 1
      size += 1
      vector[size-1] = 0
    end
    vector
  end

  def self.undo_rle(dc, rle_vector, size)
    result = []
    result[0] = dc
    result_size = 1
    for i in 0..(rle_vector.size-1)
      if(rle_vector[i][:value] != 0)
        result = fill_with_zeros(result, result_size, rle_vector[i][:skip])
        result_size += rle_vector[i][:skip]
        result[result_size] = rle_vector[i][:value]
        result_size += 1
      else
        result = fill_with_zeros(result, result_size, size-result_size)
      end
    end

    result
  end
end


describe Rle do
  describe "#do_rle" do
    it "don't skip the first element when it's non zero" do
      vector = %w{1 2 3 4}.map(&:to_i)
      Rle.do_rle(vector)[0][:value].should == 2
      Rle.do_rle(vector)[0][:skip].should == 0
    end

    it "skips any number of first zeros" do
      vector = %w{1 0 0 4 2 5}.map(&:to_i)
      Rle.do_rle(vector)[0][:value].should == 4
      Rle.do_rle(vector)[0][:skip].should == 2
    end

    it "skips zeros inside the vector" do
      vector = %w{1 1 2 0 0 5 6}.map(&:to_i)
      Rle.do_rle(vector).should == [
        { skip: 0, value: 1},
        { skip: 0, value: 2},
        { skip: 2, value: 5},
        { skip: 0, value: 6}
      ]
    end

    it "send a termination code (0,0) when all following values are 0" do
      vector = %w{1 1 2 3 0 0 0}.map(&:to_i)
      Rle.do_rle(vector).should == [
        { skip: 0, value: 1},
        { skip: 0, value: 2},
        { skip: 0, value: 3},
        { skip: 0, value: 0}
      ]
    end
  end

  describe "#fill_with_zeros" do
    it "fill with zeros with the desired amount" do
      vector = [ 1, 3, 4 ]
      Rle.fill_with_zeros(vector, 3, 3).should == [1,3,4,0,0,0]
    end
  end

  describe "#undo_rle" do
    it "decodes an rle vector" do
      dc = 1
      rle_vector = [
        { skip: 0, value: 1},
        { skip: 0, value: 2},
        { skip: 2, value: 5},
        { skip: 0, value: 6}
      ]
      Rle.undo_rle(dc, rle_vector, 6).should == [1, 1, 2, 0, 0, 5, 6]
    end

    it "fill with zero if it finds the termination code" do
      dc = 1
      rle_vector = [
        { skip: 0, value: 1},
        { skip: 0, value: 2},
        { skip: 2, value: 5},
        { skip: 0, value: 6},
        { skip: 0, value: 0}
      ]

      Rle.undo_rle(dc, rle_vector, 10).should == [1, 1, 2, 0, 0, 5, 6, 0, 0, 0]
    end
  end

  describe "#next_non_zero" do
    it "return the index of the next non zero element" do
      vector = %w{1 2 0 0 3 2 3}.map(&:to_i)
      Rle.next_non_zero(vector, 2).should == 4
    end

    it "returns -1 if the search for non-zero reaches the end of the vec" do
      vector = %w{1 2 0 0 0 0 0}.map(&:to_i)
      Rle.next_non_zero(vector, 2).should == -1
    end
  end
end
