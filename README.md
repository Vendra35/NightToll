# NightToll

A "Papers, Please"-style inspection game built entirely from scratch in Unreal Engine 5 using C++. Unlike template-based projects, NightToll features custom-designed architecture, focusing on complex state management, interactive UI systems, and deductive reasoning mechanics.

## Core Features
*   **Custom Game Architecture:** Built from the ground up without relying on standard Unreal templates.
*   **Inspection Mechanics:** Deep, logic-based gameplay where players must cross-reference information, check documents, and make critical decisions under pressure.
*   **Dynamic UI Systems:** Seamless integration of C++ backend logic with Unreal's UMG UI Designer for interactive document handling.
*   **State Management:** Robust tracking of player choices, NPC states, and daily progression.

## Tech Stack
*   **Engine:** Unreal Engine 5
*   **Language:** C++
*   **IDE:** Visual Studio 2022 / 2026

## Current Status
The core gameplay loop is fully functional:
- `TollManager` spawns a vehicle and assigns it a spline path.
- The vehicle travels the spline and reports arrival exactly once (guarded
  against duplicate reports via a boolean flag).
- On arrival, the manager spawns a document containing the driver's data.
- The player can pick up, inspect, and approve or reject the document
  through a custom UMG interface — or return it to its exact original
  desk position if they change their mind.

*(For more information about updates and the current status please see the commits section.)*
