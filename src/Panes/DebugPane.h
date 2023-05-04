/*
Copyright 2022-2022 Stephane Cuillerdier (aka aiekick)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <Panes/Abstract/AbstractPane.h>
#include <Interfaces/NodeInterface.h>
#include <Graph/Graph.h>
#include <imgui/imgui.h>
#include <stdint.h>
#include <string>
#include <memory>
#include <map>

class ProjectFile;
class DebugPane : public AbstractPane, public NodeInterface
{
private:
	BaseNodeWeak m_NodeToDebug;

public:
	bool Init() override;
	void Unit() override;
	int DrawPanes(const uint32_t& vCurrentFrame, int vWidgetId, std::string vUserDatas, PaneFlags& vInOutPaneShown) override;
	void DrawDialogsAndPopups(const uint32_t& vCurrentFrame, std::string vUserDatas) override;
	int DrawWidgets(const uint32_t& vCurrentFrame, int vWidgetId, std::string vUserDatas) override;
	void Select(BaseNodeWeak vObjet) override;

public: // singleton
	static std::shared_ptr<DebugPane> Instance()
	{
		static std::shared_ptr<DebugPane> _instance = std::make_shared<DebugPane>();
		return _instance;
	}

public:
	DebugPane(); // Prevent construction
	DebugPane(const DebugPane&) = default; // Prevent construction by copying
	DebugPane& operator =(const DebugPane&) { return *this; }; // Prevent assignment
	~DebugPane(); // Prevent unwanted destruction};
};
