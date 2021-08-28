#pragma once

#include "figure.h"
#include "../common/uncopyable.h"

#include <vector>
#include <memory>


BEGIN_NAMESPACE(WndDesign)


class FigureQueue : public Uncopyable {
private:
	template<class T> using unique_ptr = std::unique_ptr<T>;
	template<class T> using vector = std::vector<T>;

private:
	Vector offset = vector_zero;
	vector<Vector> group_offset_stack;
public:
	void PushOffset(Vector offset) { this->offset += offset; }
	void PopOffset(Vector offset) { PushOffset(-offset); }

private:
	struct FigureItem {
		Point offset;
		unique_ptr<const Figure> figure;
	};
	vector<FigureItem> figures;
public:
	const vector<FigureItem>& GetFigures() const { return figures; }
public:
	void add(Point offset, unique_ptr<const Figure> figure) {
		figures.emplace_back(FigureItem{ offset + this->offset, std::move(figure) });
	}
	void add(Point offset, alloc_ptr<const Figure> figure) {
		add(offset, unique_ptr<const Figure>(figure));
	}
	template<class FigureType, class ...Types, class = std::enable_if_t<std::is_base_of_v<Figure, FigureType>>>
	void add(Point offset, Types&&... args) {
		add(offset, std::make_unique<FigureType>(std::forward<Types>(args)...));
	}

private:
	struct FigureGroup {
		union {
			struct { // as group begin
				uint group_end_index;
				uint figure_index;
				Vector coordinate_offset;
				Rect clip_region;
			};
			struct { // as group end
				uint null_index;  // == -1
				uint figure_index;
				mutable Vector prev_coordinate_offset;
				mutable Rect prev_clip_region;
			};
		};
		bool IsBegin() const { return group_end_index != (uint)-1; }
	};
	vector<FigureGroup> groups;
public:
	void CheckFigureGroup() const { if (!group_offset_stack.empty()) { throw std::logic_error("unbalanced figure group"); } }
	const vector<FigureGroup>& GetFigureGroups() const { return groups; }
public:
	uint BeginGroup(Vector coordinate_offset, Rect clip_region) {
		uint group_begin_index = (uint)groups.size();
		groups.push_back(FigureGroup{ (uint)-1, (uint)figures.size(), coordinate_offset + offset, clip_region });
		group_offset_stack.push_back(offset); offset = vector_zero;
		return group_begin_index;
	}
	void EndGroup(uint group_begin_index) {
		if (group_begin_index >= groups.size() || groups[group_begin_index].group_end_index != -1) { throw std::invalid_argument("invalid group begin index"); }
		groups[group_begin_index].group_end_index = (uint)groups.size();
		offset = group_offset_stack.back(); group_offset_stack.pop_back();
		groups.push_back(FigureGroup{ (uint)-1, (uint)figures.size(), vector_zero, region_empty });
	}
};


END_NAMESPACE(WndDesign)