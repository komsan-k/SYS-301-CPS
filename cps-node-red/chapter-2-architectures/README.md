\chapter{CPS Architectures and Models}
\label{chap:cps-architectures}

\section*{Chapter Overview}
This chapter explores the architectural frameworks and system models that underpin Cyber-Physical Systems (CPS). Building on the conceptual foundation from Chapter~\ref{chap:cps-foundations}, we examine reference architectures (5C, IIRA, RAMI~4.0), layered abstractions, discrete-event and hybrid models, and co-design principles that align hardware, software, and communication. Practical work introduces modeling CPS data flow in Node-RED without physical hardware, emphasizing data injection, transformation, and visualization.

\subsection*{Learning Outcomes}
After completing this chapter, students will be able to:
\begin{enumerate}
  \item Describe layered CPS architectures and their functional decomposition.
  \item Explain the 5C architecture (Connection, Conversion, Cyber, Cognition, Configuration).
  \item Contrast IIRA (Industrial Internet Reference Architecture) with RAMI~4.0.
  \item Model CPS processes using discrete-event (DE) and hybrid system representations.
  \item Build a simple data flow model in Node-RED using inject, function, and dashboard nodes.
\end{enumerate}

\subsection*{Key Terms}
5C architecture, IIRA, RAMI~4.0, discrete-event system (DES), hybrid system, layered CPS model, co-simulation, Node-RED dataflow.

\section{Reference Architectures for CPS}

\subsection{The 5C Architecture}
The 5C architecture (Lee et al., 2015) is widely used in smart manufacturing and CPS engineering:
\begin{enumerate}
  \item \textbf{Connection:} sensors and devices collect raw data.
  \item \textbf{Conversion:} raw data are processed into useful information.
  \item \textbf{Cyber:} information is aggregated, analyzed, and stored in cyber infrastructure.
  \item \textbf{Cognition:} decision-making and knowledge generation.
  \item \textbf{Configuration:} adaptive feedback to physical systems.
\end{enumerate}
This stack captures both bottom-up (data-driven) and top-down (decision-driven) interactions.

\subsection{IIRA (Industrial Internet Reference Architecture)}
IIRA, developed by the Industrial Internet Consortium, specifies three viewpoints:
\begin{itemize}
  \item \textbf{Business:} stakeholders, values, ROI, compliance.
  \item \textbf{Usage:} functional requirements, scenarios, workflows.
  \item \textbf{Functional:} capabilities organized into control, operations, information, and business domains.
\end{itemize}
IIRA emphasizes interoperability and cross-industry adaptability.

\subsection{RAMI~4.0 (Reference Architectural Model Industry 4.0)}
RAMI~4.0 defines a three-dimensional reference cube:
\begin{description}
  \item[Hierarchy levels:] from field devices to enterprise.
  \item[Layers:] asset, integration, communication, information, functional, business.
  \item[Life cycle/value stream:] type (design) to instance (runtime).
\end{description}
RAMI~4.0 stresses life cycle integration and standardized interfaces.

\section{Layered CPS Models}
A simplified CPS layered model helps organize analysis:

\begin{figure}[h]
\centering
\begin{tikzpicture}[
  node/.style={draw, rounded corners=2pt, align=center, minimum height=8mm, inner sep=4pt},
  lay/.style={node, minimum width=11cm},
  >={Latex[length=2mm]}
]
\node[lay, fill=purple!10] (human) {Human Layer (Operators, Dashboards, Alarms)};
\node[lay, fill=green!10, below=2mm of human] (app) {Application Layer (Analytics, Decision Support, Digital Twin)};
\node[lay, fill=orange!12, below=2mm of app] (cyber) {Cyber Layer (Estimation, Control, Coordination, Computation)};
\node[lay, fill=blue!10, below=2mm of cyber] (comm) {Communication Layer (MQTT/OPC~UA, Networks, Security)};
\node[lay, fill=teal!10, below=2mm of comm] (percep) {Perception Layer (Sensors, Signal Processing)};
\node[lay, fill=cyan!15, below=2mm of percep] (act) {Actuation Layer (Motors, Drivers, Actuators)};

\draw[->] (percep) -- (comm);
\draw[->] (comm) -- (cyber);
\draw[->] (cyber) -- (app);
\draw[->] (app) -- (human);
\draw[->] (cyber) -- (comm);
\draw[->] (comm) -- (act);
\end{tikzpicture}
\caption{Simplified layered CPS model (perception to actuation).}
\label{fig:cps-layered}
\end{figure}

\section{System Models: DES, Continuous, and Hybrid}
\paragraph{Discrete-Event Systems (DES).}  
CPS often include events: packet arrivals, threshold crossings, alarms, or mode changes. DES model system state transitions triggered by discrete events.

\paragraph{Continuous Models.}  
Physical plants (temperature, velocity, voltage) are modeled by differential equations.  

\paragraph{Hybrid Models.}  
Combine both perspectives. Example: traffic lights — continuous vehicle arrivals (flow), discrete switching of lights.

\section{CPS Co-Design Principles}
Key principles:
\begin{enumerate}
  \item \textbf{Hardware-software co-design:} partitioning functions across microcontrollers, edge servers, and cloud.
  \item \textbf{Timing-aware design:} allocate latency budgets across sensing, networking, computation, and actuation.
  \item \textbf{Safety/security-first:} design invariants and fail-safe modes early.
  \item \textbf{Scalable architecture:} anticipate growth in nodes and data volume.
\end{enumerate}

\section{Case Study: Traffic Light CPS}
\begin{itemize}
  \item \textbf{Sensors:} inductive loops or cameras detect cars.
  \item \textbf{Controllers:} compute phases based on arrival rates.
  \item \textbf{Actuators:} lights switch on/off.
  \item \textbf{Communication:} intersections share load information over network.
  \item \textbf{Model:} hybrid system with continuous vehicle arrivals and discrete FSM of traffic lights.
\end{itemize}

\section{Node-RED Lab 2: Modeling CPS Data Flow}
\label{lab:modeling-cps}

\subsection*{Objectives}
\begin{itemize}
  \item Model a CPS pipeline using Node-RED without physical devices.
  \item Use inject nodes as simulated sensors.
  \item Transform data using function nodes.
  \item Display actuator states on a dashboard.
\end{itemize}

\subsection*{Required Software}
Node-RED (with dashboard package).

\subsection*{Procedure}
\begin{enumerate}
  \item Drag two \textbf{inject} nodes: label them ``Sensor~A'' and ``Sensor~B''.
  \item Connect both to a \textbf{function} node (Processing). Example code:
\begin{verbatim}
let v = Number(msg.payload);
msg.payload = {source: msg.topic, value: v, score: v*2};
return msg;
\end{verbatim}
  \item Connect function node to:
    \begin{itemize}
      \item a \textbf{gauge} node (dashboard) showing score,
      \item and a \textbf{debug} node to observe messages.
    \end{itemize}
  \item Deploy, trigger sensors, and observe dashboard.
\end{enumerate}

\subsection*{Expected Output}
When Sensor~A injects value 42, the gauge displays 84. When Sensor~B injects 17, the gauge shows 34. Debug panel prints JSON objects with source, value, score.

\subsection*{Assessment Rubric (10 points)}
\begin{itemize}
  \item[2] Correct creation of inject nodes with topics.
  \item[3] Function node transforms payload correctly.
  \item[3] Gauge displays processed score.
  \item[2] Debug messages consistent with inputs.
\end{itemize}

\subsection*{Starter Flow (Importable JSON)}
\begin{verbatim}
[{"id":"tab2","type":"tab","label":"Lab2 - CPS Model"},
{"id":"injA","type":"inject","z":"tab2","name":"Sensor A","props":[{"p":"payload"},{"p":"topic","vt":"str"}],
"topic":"sensor/A","payload":"42","payloadType":"num","x":130,"y":100,"wires":[["fn2"]]},
{"id":"injB","type":"inject","z":"tab2","name":"Sensor B","props":[{"p":"payload"},{"p":"topic","vt":"str"}],
"topic":"sensor/B","payload":"17","payloadType":"num","x":130,"y":160,"wires":[["fn2"]]},
{"id":"fn2","type":"function","z":"tab2","name":"Processing","func":"let v = Number(msg.payload);\nmsg.payload = {source: msg.topic, value: v, score: v*2};\nreturn msg;","outputs":1,"noerr":0,"x":330,"y":130,"wires":[["uiGauge","dbg2"]]},
{"id":"uiGauge","type":"ui_gauge","z":"tab2","group":"uigrp2","order":1,"gtype":"gage","title":"Actuator Level","label":"units","format":"{{msg.payload.score}}","min":0,"max":"100","x":530,"y":130,"wires":[]},
{"id":"dbg2","type":"debug","z":"tab2","name":"debug","active":true,"tosidebar":true,"complete":"true","x":530,"y":90,"wires":[]},
{"id":"uigrp2","type":"ui_group","name":"Lab2","tab":"uitab2","order":1,"disp":true,"width":"6","collapse":false},
{"id":"uitab2","type":"ui_tab","name":"CPS Labs","icon":"dashboard","disabled":false,"hidden":false}]
\end{verbatim}

\section*{Summary}
CPS architectures organize sensing, computation, communication, and actuation into layered stacks, with reference models (5C, IIRA, RAMI~4.0) providing systematic guidance. Modeling frameworks (DES, hybrid systems) capture the interplay between discrete and continuous dynamics. Node-RED offers a rapid way to prototype CPS dataflows.

\section*{Review Questions}
\begin{enumerate}
  \item What are the five layers of the 5C architecture? Give an example for each.
  \item How does IIRA’s business/usage/functional viewpoint differ from RAMI~4.0’s cube?
  \item Distinguish between discrete-event, continuous, and hybrid CPS models.
  \item Why is co-design (hardware + software) critical in CPS?
\end{enumerate}

\section*{Exercises}
\begin{enumerate}
  \item Extend the Lab~\ref{lab:modeling-cps} flow by adding a third inject node (Sensor~C). Apply a different processing rule (e.g., square the value) and display on a second gauge.
  \item Sketch the RAMI~4.0 cube and map a smart factory use case onto it.
  \item Model a traffic light CPS as a hybrid system with vehicle flow (continuous) and traffic lights (discrete FSM).
\end{enumerate}

\section*{Further Reading}
\begin{itemize}
  \item L. Monostori, ``Cyber-Physical Production Systems: Roots, Expectations and R\&D Challenges,'' Procedia CIRP, 2014.
  \item Industrial Internet Consortium, \emph{Industrial Internet Reference Architecture (IIRA)}, 2019.
  \item Plattform Industrie 4.0, \emph{RAMI~4.0 Reference Model}, 2015.
  \item P. Derler, E.A. Lee, A. Sangiovanni-Vincentelli, ``Modeling Cyber-Physical Systems,'' Proc. IEEE, 2012.
\end{itemize}
