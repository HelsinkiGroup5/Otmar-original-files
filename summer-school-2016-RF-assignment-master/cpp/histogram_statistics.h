//
//  histogram_statistics.h
//  DistRandomForest
//
//  Created by Benjamin Hepp.
//
//

#pragma once

#include <vector>
#include <cmath>
#include <numeric>

#include "ait.h"

namespace ait
{

/// @brief A histogram over the classes of samples.
class HistogramStatistics
{
public:

    class Factory
    {
        size_type num_of_classes_;

    public:
        using value_type = HistogramStatistics;

        Factory(size_type num_of_classes)
        : num_of_classes_(num_of_classes)
        {}

        HistogramStatistics create() const
        {
            return HistogramStatistics(num_of_classes_);
        }
    };
    
	using HistogramType = std::vector<size_type>;

    /// @brief Create an empty histogram.
    HistogramStatistics()
    : histogram_(0, 0), num_of_samples_(0)
    {}

    /// @brief Create an empty histogram.
    /// @param num_of_classes The number of classes.
    HistogramStatistics(size_type num_of_classes)
    : histogram_(num_of_classes, 0), num_of_samples_(0)
    {}

    /// @brief Create a histogram from a vector of counts per class.
    HistogramStatistics(const std::vector<size_type>& histogram)
    : histogram_(histogram),
    num_of_samples_(std::accumulate(histogram.cbegin(), histogram.cend(), 0))
    {}

    void lazy_accumulate(size_type label) {
        assert(label < histogram_.size());
        histogram_[label]++;
    }

	template <typename TSample>
    void lazy_accumulate(const TSample& sample)
    {
        size_type label = sample.get_label();
        lazy_accumulate(label);
    }

    void finish_lazy_accumulation()
    {
        compute_num_of_samples();
    }

    void accumulate(size_type label) {
        assert(label < histogram_.size());
        histogram_[label]++;
        num_of_samples_++;
    }

	template <typename TSample>
    void accumulate(const TSample& sample)
    {
        size_type label = sample.get_label();
        accumulate(label);
    }
    
    void accumulate(const HistogramStatistics& statistics)
    {
        assert(histogram_.size() == statistics.histogram_.size());
        for (size_type i=0; i < statistics.histogram_.size(); i++)
        {
            histogram_[i] += statistics.histogram_[i];
        }
		num_of_samples_ += statistics.num_of_samples();
    }

    template <typename T>
    void accumulate(T it_start, T it_end)
    {
        for (T it = it_start; it != it_end; ++it)
        {
            lazy_accumulate(*it);
        }
        finish_lazy_accumulation();
    }

    template <typename T>
    void accumulate_histograms(T it_start, T it_end)
    {
        for (T it = it_start; it != it_end; ++it)
        {
            assert(histogram_.size() == *it.histogram_.size());
            for (size_type i=0; i < it->histogram_.size(); i++)
            {
                histogram_[i] += it->histogram_[i];
            }
        }
        finish_lazy_accumulation();
    }

    /// @brief Return the numbers of samples contributing to the histogram.
    size_type num_of_samples() const
    {
        return num_of_samples_;
    }

	/// @brief Return the number of bins in the histogram
	size_type num_of_bins() const
	{
		return histogram_.size();
	}

    /// @brief Return the vector of counts per class.
    const std::vector<size_type>& get_histogram() const
    {
        return histogram_;
    }

    /// @return: The Shannon entropy of the histogram.
    const scalar_type entropy() const
    {
        scalar_type entropy = 0;
        // TODO UIE A2: Compute entropy of histogram_ member
        // Hint: Use C++11 auto type specifier for iterators of histogram_ member
        // ...
        return entropy;
    }

	/// @return: Return bin with maximum number of samples
	size_type get_max_bin() const {
		size_type max_bin = std::max_element(histogram_.cbegin(), histogram_.cend()) - histogram_.cbegin();
		return max_bin;
	}
private:
    void compute_num_of_samples()
    {
        num_of_samples_ = std::accumulate(histogram_.cbegin(), histogram_.cend(), 0);
    }
    
#ifdef SERIALIZE_WITH_BOOST
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive& archive, const unsigned int version, typename enable_if_boost_archive<Archive>::type* = nullptr)
    {
        archive & histogram_;
        archive & num_of_samples_;
    }
#endif
    
    friend class cereal::access;
    
    template <typename Archive>
    void serialize(Archive& archive, const unsigned int version, typename disable_if_boost_archive<Archive>::type* = nullptr)
    {
        archive(cereal::make_nvp("histogram", histogram_));
        archive(cereal::make_nvp("num_of_samples", num_of_samples_));
    }

    HistogramType histogram_;
    size_type num_of_samples_;
};

}
