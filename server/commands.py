from dataclasses import dataclass, field
from typing import Tuple, List, Any

from models import Mode

@dataclass
class ProgramStep:
    mode: Tuple[Mode, Mode]
    time: int


@dataclass
class ProgramCommand:
    contents: List[ProgramStep]
    type: str = "program"


@dataclass
class ManualCommand:
    contents: bool
    type: str = "manual"


@dataclass
class MaintenanceCommand:
    contents: bool
    type: str = "maintenance"


@dataclass
class SetModeCommand:
    contents: Tuple[Mode, Mode]
    type: str = "set"
