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
public:
	void Offset(Vector offset, std::function<void(void)> func) {
		this->offset += offset; 
		func();
		this->offset -= offset;
	}

	// transform
private:
	Transform transform;
public:
	Scale GetScale() const { return transform.GetScale(); }

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
				Transform prev_transform;
			};
		};
		bool IsBegin() const { return group_end_index != (uint)-1; }
	};
	vector<FigureGroup> groups;
public:
	const vector<FigureGroup>& GetFigureGroups() const { return groups; }
public:
	void Group(Transform group_transform, Rect clip_region, std::function<void(void)> func) {
		uint group_begin_index = (uint)groups.size();
		Transform prev_transform = transform;
		groups.push_back({ (uint)-1, (uint)figures.size(), transform = group_transform * offset * transform, clip_region });
		offset_stack.push_back(offset); offset = vector_zero;
		func();
		groups[group_begin_index].group_end_index = (uint)groups.size();
		groups.push_back({ (uint)-1, (uint)figures.size(), transform = prev_transform, region_empty });
		offset = offset_stack.back(); offset_stack.pop_back();
	}

public:
	FigureQueue(std::function<void(FigureQueue&)> func) {
		Group(vector_zero, region_infinite, [&]() { func(*this); });
	}
};


END_NAMESPACE(WndDesign)