from openpyxl import Workbook
wb = Workbook()
ws = wb.active
ws['A1'] = 42
ws.append([1, 2, 3])


from retry_requests import retry
my_session = retry()


import numpy as np
from pyrealm.pmodel import PModelEnvironment, PModel

env = PModelEnvironment(
    tc=np.array([20]), vpd=np.array([1000]),
    co2=np.array([400]), patm=np.array([101325]),
    fapar=1, ppfd=300
    )

pmodel_c3 = PModel(env)
pmodel_c3.gpp
