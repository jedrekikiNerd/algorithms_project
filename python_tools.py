import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

# Ścieżka do folderu, w którym znajdują się pliki z pomiarami
folder_path = 'tests_results'
current_directory = os.path.dirname(__file__)
folder_path = os.path.join(current_directory, folder_path)


# Nazwy plików do iteracji i wczytywania
endings = ["1_1.txt"]
operation_names = ["quicksort", "mergesort", "bucketsort", "find_and_delete_empty", "average_values", "center_values"]
operation_name_map = {"quicksort": "Algorytmu Quicksort", "mergesort": "Algorytmu Mergesort", "bucketsort": "Algorytmu Bucketsort", "find_and_delete_empty": "Czasu znajdywania i usuwania pustych rankingów", "average_values": "Średnich wartości", "center_values": "Median"}

# Funkcja do wczytywania danych z plików tekstowych i obliczania średniej
def process_files_in_folder(folder_path):
    data = {}
    # Iteruj przez pliki w folderze
    for operation in operation_names:
        operation_name = operation
        # Jeśli nie istnieje jeszcze klucz w słowniku, stwórz go
        if operation not in data:
            data[operation] = {"sizes":[], "times": []}
        for ending in endings:
            # Wczytaj dane z pliku
            file_path = os.path.join(folder_path, operation+ending)
            df = pd.read_csv(file_path, delimiter=';', header=None, names=['size', 'time'])

            # lista 'times' musi mieć odpowiednią ilość elementów
            while len(data[operation_name]['times']) < len(df['time']):
                data[operation_name]['times'].append(0)

            for i, time_value in enumerate(df['time']):
                data[operation_name]['times'][i] += time_value
            data[operation_name]['sizes'] = df['size'].to_list()

        for i, time_value in enumerate(data[operation_name]['times']):
            data[operation_name]['times'][i] /= len(endings)

    return data

data = process_files_in_folder(folder_path)
for operation in data:
    pass
    print(f"{operation}: {data[operation]}")

# Funkcja modelująca n*log(n), gdzie a jest parametrem skalującym
def model_func(n, a):
    # Upewnienie się, że n jest typu np.ndarray
    n = np.array(n, dtype=float)
    return a * n * np.log(n)

def plot_operation_trend(data, operation):
    sizes = data[operation]['sizes']
    times = data[operation]['times']
    
    # Rysowanie wykresu
    plt.figure(figsize=(10, 6))
    
    # Obliczanie krzywej trendu
    z = np.polyfit(sizes, times, 1)
    p = np.poly1d(z)
    r_squared = ""
    trend_line_eq = ""
    '''
    if f"{z[0]:.2f}" in ["0.00", "-0.00"]:
        p = np.mean(times)
        plt.plot(sizes, p*np.ones_like(times), "r--", label='Linia trendu')
        # Obliczanie R^2
        #r_squared = f"{np.corrcoef(times, p*np.ones_like(times))[0, 1]**2}"
        trend_line_eq = f'y = {p:.2f}'
        plt.text(0.75 * max(sizes), 0.05 * max(times), f'{trend_line_eq}', fontsize=11)
    
    plt.plot(sizes, p(sizes), "r--", label='Linia trendu')
    # Obliczanie R^2
    r_squared = f"{(np.corrcoef(times, p(sizes))[0, 1]**2):.2f}"
    trend_line_eq = f'y = {z[0]:.2f}x + {z[1]:.2f}'
    plt.text(0.75 * max(sizes), 0.05 * max(times), f'{trend_line_eq}\n$R^2 = {r_squared}$', fontsize=11)
    '''

    # Dopasowanie modelu do danych
    params, params_covariance = curve_fit(model_func, sizes, times, p0=[1])

    # Obliczanie krzywej trendu
    predicted_times = model_func(sizes, params[0])
    # Wykres danych i krzywej trendu
    if operation in ["mergesort", "quicksort"]:
        plt.plot(sizes, predicted_times, 'r--', label='Krzywa trendu $a \cdot n\log(n)$')  # Krzywa trendu
        # Obliczanie R^2
        residuals = times - predicted_times
        ss_res = np.sum(residuals**2)
        ss_tot = np.sum((times - np.mean(times))**2)
        r_squared = 1 - (ss_res / ss_tot)
        plt.text(0.75 * max(sizes), 0.15 * max(times), f'$a = {params[0]:.4f}$\n$R^2 = {r_squared}$', fontsize=11)
    elif operation in ["bucketsort"]:
        plt.plot(sizes, p(sizes), "r--", label='Linia trendu')
        # Obliczanie R^2
        r_squared = f"{(np.corrcoef(times, p(sizes))[0, 1]**2):.2f}"
        trend_line_eq = f'y = {z[0]:.2f}x + {z[1]:.2f}'
        plt.text(0.75 * max(sizes), 0.05 * max(times), f'{trend_line_eq}\n$R^2 = {r_squared}$', fontsize=11)

    n = np.array(sizes, dtype=float)
    # Obliczenia
    o_n = n
    o_nlogn = n * np.log(n)

    if operation == "quicksort":
        o_n = o_n * 1000
        o_nlogn = o_nlogn * 1000
    elif operation == "mergesort":
        o_n = o_n * 100
        o_nlogn = o_nlogn * 100
    elif operation == "bucketsort":
        o_n = o_n * 10
        o_nlogn = o_nlogn * 10

    plt.plot(n, o_n, 'y--', label='O(n)')
    plt.plot(n, o_nlogn, 'g--', label='O(n log(n))')
    
    plt.plot(sizes, times, label=f'{operation}', linestyle='-', marker='o')  # Linia ciągła z punktami
    
    plt.xlabel('Wielkość danych [ilość wczytanych liczb]')
    plt.ylabel('Czas wykonania [ns]')
    plt.title(f'Czas wykonania operacji {operation_name_map[operation]}')
    plt.legend()

    plt.xscale('linear')
    plt.yscale('linear')
    plt.xlim(left=0, right=max(sizes)+max(sizes)/100)  # Ustawienie zakresu osi X
    plt.ylim(bottom=0, top=max(times)+max(max(times)/10, 200))
    
    # Zapis do pliku (upewnij się, że zmienna `current_directory` jest zdefiniowana)
    current_directory = '.'  # Zaktualizuj tę ścieżkę do aktualnego katalogu roboczego
    file_name = f"{operation}.png"
    full_path = os.path.join(current_directory, "figures1", file_name)
    plt.savefig(full_path, dpi=300)
    
    plt.show()
    plt.close()


# Funkcja do wyświetlania wykresu i krzywej trendu dla danych operacji i struktury danych
def plot_operation_trend_sizes(data, data_structure):
    sizes = data[data_structure]['sizes']
    times = data[data_structure]['dt_sizes']
    
    # Rysowanie wykresu
    plt.figure(figsize=(10, 6))
    plt.margins(0.1)
    
    # Obliczanie krzywej trendu
    z = np.polyfit(sizes, times, 1)
    p = np.poly1d(z)
    #plt.plot(sizes, p(sizes), "r--", label='Linia trendu')
    plt.plot(sizes, times, label=data_structure_name_map[data_structure], linestyle='-', marker='o')  # Linia ciągła z punktami
    
    plt.xlabel('Wielkość danych [ilość wczytanych liczb]')
    plt.ylabel('Ilość zajętej pamięci [B]')
    plt.title(f'Ilość zajętej pamięci przez strukturę danych w zależności od ilości wczytanych danych')
    plt.legend()

    # Ustawienia osi
    plt.xscale('linear')  # Zmiana skali osi X
    plt.yscale('linear')  # Zmiana skali osi Y
    plt.xlim(left=0, right=max(sizes)+max(sizes)/100)  # Ustawienie zakresu osi X
    plt.ylim(bottom=0, top=max(times)+max(max(times)/10, 200))
    # Zmiana formatowania osi na liczby całkowite (bez notacji naukowej)
    plt.ticklabel_format(style='plain', axis='both')

    # Zapis do pliku
    file_name = f"{data_structure}_size.png"
    full_path = os.path.join(current_directory, "figures3")
    full_path = os.path.join(full_path, file_name)
    plt.savefig(full_path, dpi=300)
    plt.show()
    plt.close()  # Zamknięcie figury, żeby nie zajmowała pamięci RAM


# Funkcja do wyświetlania wykresu i krzywej trendu dla danych operacji i struktury danych
def plot_operation_trend2(data):
    sizes = data["quicksort"]['sizes']
    max_time = 0

    plt.figure(figsize=(10, 6))
    plt.margins(0.1)
    #plt.xscale('linear')  # Zmiana skali osi X
    #plt.yscale('log')  # Zmiana skali osi Y

    times = data["quicksort"]['times']
    print(times)
    plt.plot(sizes, times, label='Algorytm Quicksort', linestyle='-', marker='o', color="purple")  # Linia ciągła z punktami
    max_time = max(max_time, max(times))

    times = data["mergesort"]['times']
    plt.plot(sizes, times, label='Algorytm Mergesort', linestyle='-', marker='o', color="blue")  # Linia ciągła z punktami
    max_time = max(max_time, max(times))

    times = data["bucketsort"]['times']
    plt.plot(sizes, times, label='Algorytm Bucketsort', linestyle='-', marker='o', color="green")  # Linia ciągła z punktami
    max_time = max(max_time, max(times))

    n = np.array(sizes, dtype=float)
    # Obliczenia
    o_n = n
    o_n2 = n*n
    o_nlogn = n * np.log(n)
    plt.plot(n, o_n, 'y--', label='O(n)')
    plt.plot(n, o_nlogn, 'g--', label='O(n log(n))')
    plt.plot(n, o_n2, 'r--', label='O($n^2$)')

    # Obliczanie krzywej trendu
    #z = np.polyfit(sizes, times, 1)
    #p = np.poly1d(z)
    #plt.plot(sizes, p(sizes), "r--", label='Linia trendu')
    
    plt.xlabel('Wielkość danych [ilość wczytanych liczb]')
    plt.ylabel('Czas wykonania [ns]')
    plt.title(f'Porównanie czasu wykonania wszystkich algorytmów sortujących')
    plt.legend()

    # Ustawienia osi
    plt.xscale('linear')  # Zmiana skali osi X
    plt.yscale('log')  # Zmiana skali osi Y
    #plt.xlim(left=0, right=max(sizes)+max(sizes)/100)  # Ustawienie zakresu osi X
    #plt.ylim(bottom=0, top=max_time+max(max_time/10, 200))
    # Zmiana formatowania osi na liczby całkowite (bez notacji naukowej)

    # Zapis do pliku
    file_name = f"{operation}.png"
    full_path = os.path.join(current_directory, "figures3")
    full_path = os.path.join(full_path, file_name)
    #plt.savefig(full_path, dpi=300)
    plt.show()
    
    #plt.close()  # Zamknięcie figury, żeby nie zajmowała pamięci RAM


# Iteracja przez dane i rysowanie wykresów dla każdej operacji i struktury danych
for operation in data:
    pass
    #plot_operation_trend(data, operation)

plot_operation_trend2(data)

size_data = {}




#for operation in data[data_structure]:
#    plot_operation_trend(data, data_structure, operation)

# Generowanie tabeli LaTeX
def generate_latex_tables_with_borders(data):
    for operation in set(op for ds in data.values() for op in ds):
        table_content = ""
        sizes = sorted(set(size for ds in data.values() for op in ds.get(operation, {}) for size in ds[operation]['sizes']))
        for size in sizes:
            row = [str(size)]
            for structure in data:
                if operation in data[structure] and len(data[structure][operation]['sizes']) > 0:
                    index = data[structure][operation]['sizes'].index(size)
                    time = data[structure][operation]['times'][index]
                    row.append(f"{time:.0f}")
                else:
                    row.append("—")
            table_content += " & ".join(row) + " \\\\ \\hline" + "\n"
        table_latex = f"""
\\begin{{table}}[ht]
\\centering
\\begin{{tabular}}{{|{'|'.join('c' for _ in range(len(data)+1))}|}}
\\hline
\\textbf{{Wielkość danych}} & \\textbf{{{'} & \\textbf{'.join(data.keys())} \\\\
\\hline
{table_content}\\end{{tabular}}
\\caption{{Średni czas wykonania operacji {operation_name_map[operation]}}}
\\label{{tab:{operation_name_map[operation]}}}
\\end{{table}}
"""
        print(table_latex)


def generate_latex_tables(data):
    # Zbieranie średnich czasów wykonania dla każdej operacji dla każdej struktury
    avg_times = {}
    operations_set = set(op for ds in data.values() for op in ds)
    for operation in operations_set:
        avg_times[operation] = {}
        for structure, operations in data.items():
            if operation in operations:
                avg_time = sum(operations[operation]['times']) / len(operations[operation]['times'])
                avg_times[operation][structure] = avg_time
            else:
                avg_times[operation][structure] = "—"

    # Tworzenie tabeli LaTeX
    table_content = ""
    for operation in avg_times:
        row = [operation]
        for structure in data:
            row.append(f"{avg_times[operation][structure]:.0f}")
        table_content += " & ".join(row) + " \\\\ \\hline" + "\n"
    table_latex = f"""
\\begin{{table}}[ht]
\\centering
\\begin{{tabular}}{{|{'|'.join('c' for _ in range(len(data) + 1))}|}}
\\hline
\\textbf{{Operacja}} & \\textbf{{{'} & \\textbf{'.join(data.keys())}}} \\\\
\\hline
{table_content}\\end{{tabular}}
\\caption{{Średnie czasy wykonania dla poszczególnych operacji}}
\\label{{tab:avg_times}}
\\end{{table}}
"""
    print(table_latex)

# Przykład użycia:
#generate_latex_tables(data)