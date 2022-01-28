#pragma once

#include "figure.h"
#include "../common/uncopyable.h"
#include "../geometry/transform.h"

#include <vector>
#include <memory>
#include <functional>


BEGIN_NAMESPACE(WndDesign)


class FigureQueue : public Uncopyable {
private:
	template<class T> using vector = std::vector<T>;
	template<class T> using unique_ptr = std::unique_ptr<T>;

	// offset
private:
	Vector offset = vector_zero;
	vector<Vector> offset_stack;
private:
	void PushOffset(Vector offset) { this->offset += offset; }
	void PopOffset(Vector offset) { PushOffset(-offset); }
public:
	void Offset(Vector offset, std::function<void(void)> func) {
		PushOffset(offset); func(); PopOffset(offset);
	}

	// figure
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

	// group
private:
	struct FigureGroup {
		union {
			struct {  // as group begin
				uint group_end_index;
				uint figure_index;
				Transform transform;
				Rect clip_region;
			};
			struct {  // as group end
				uint null_index;  // == -1
				uint figure_index;
				mutable Transform prev_transform;
			};
		};
		bool IsBegin() const { return group_end_index != (uint)-1; }
	};
	vector<FigureGroup> groups;
public:
	const vector<FigureGroup>& GetFigureGroups() const { return groups; }
private:
	uint BeginGroup(Transform transform, Rect clip_region) {
		uint group_begin_index = (uint)groups.size();
		groups.push_back(FigureGroup{ (uint)-1, (uint)figures.size(), transform * offset, clip_region });
		offset_stack.push_back(offset); offset = vector_zero;
		return group_begin_index;
	}
	void EndGroup(uint group_begin_index) {
		if (group_begin_index >= groups.size() || groups[group_begin_index].group_end_index != -1) { throw std::invalid_argument("invalid group begin index"); }
		groups[group_begin_index].group_end_index = (uint)groups.size();
		groups.push_back(FigureGroup{ (uint)-1, (uint)figures.size(), Transform(), region_empty });
		offset = offset_stack.back(); offset_stack.pop_back();
	}
public:
	void Group(Transform group_transform, Rect clip_region, std::function<void(void)> func) {
		uint begin = BeginGroup(group_transform, clip_region); func(); EndGroup(begin);
	}

public:
	FigureQueue(std::function<void(FigureQueue&)> func) {
		Group(vector_zero, region_infinite, [&]() { func(*this); });
	}
};


END_NAMESPACE(WndDesign)