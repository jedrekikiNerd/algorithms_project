import numpy as np
import os
import matplotlib.pyplot as plt

katalog = os.path.dirname(os.path.abspath(__file__))

# Lista nazw plików
nazwy_plikow = [
    "dijkstra_to_all_g1_0,250000.txt",
    "dijkstra_to_chosen_g1_0,250000.txt",
    "dijkstra_to_all_g2_0,250000.txt",
    "dijkstra_to_chosen_g2_0,250000.txt",
    "dijkstra_to_all_g1_0,500000.txt",
    "dijkstra_to_chosen_g1_0,500000.txt",
    "dijkstra_to_all_g2_0,500000.txt",
    "dijkstra_to_chosen_g2_0,500000.txt",
    "dijkstra_to_all_g1_0,750000.txt",
    "dijkstra_to_chosen_g1_0,750000.txt",
    "dijkstra_to_all_g2_0,750000.txt",
    "dijkstra_to_chosen_g2_0,750000.txt",
    "dijkstra_to_all_g1_1,000000.txt",
    "dijkstra_to_chosen_g1_1,000000.txt",
    "dijkstra_to_all_g2_1,000000.txt",
    "dijkstra_to_chosen_g2_1,000000.txt"
]

# Pusty słownik do przechowywania danych
dane = {}

# Wczytywanie danych z plików
for nazwa_pliku in nazwy_plikow:
    # Pełna ścieżka do pliku
    sciezka_pliku = os.path.join(katalog, nazwa_pliku)
    # Wyciąganie procentowej gęstości z nazwy pliku
    gestosc_str = nazwa_pliku.split("_")[-1].split(".")[0].replace(",", ".")
    gestosc = float(gestosc_str)
    
    # Wyciąganie nazwy grafu i algorytmu z nazwy pliku
    czesci_nazwy = nazwa_pliku.split("_")
    graf = czesci_nazwy[3]
    algorytm = czesci_nazwy[2]
    
    # Tworzenie słownika, jeśli nie istnieje
    if graf not in dane:
        dane[graf] = {}
    if algorytm not in dane[graf]:
        dane[graf][algorytm] = {}
    
    # Wczytywanie danych z pliku do tablicy numpy
    dane[graf][algorytm][gestosc] = np.loadtxt(sciezka_pliku, delimiter=";")

# Teraz dane są wczytane i przechowywane w słowniku dane, gdzie klucze to nazwy grafów, algorytmów i procentowe gęstości
# Możemy na przykład uzyskać dane dla grafu g1, algorytmu to_all i gęstości 0.25 tak: dane['g1']['to_all'][0.25]

# Funkcja do generowania tabelki LaTeX dla danych grafu i algorytmu
def generate_latex_table(graph, algorithm):
    # Pobranie danych dla danego grafu i algorytmu
    data = dane[graph][algorithm]
    
    # Nagłówki kolumn tabeli
    headers = ["Ilość wierzchołków"] + [f"Czas - gęstość {density}" for density in data.keys()]
    
    # Otwarcie tabelki LaTeX
    latex_table = "\\begin{table}[h!]\n\\centering\n\\begin{tabular}{|" + "c|" * (len(data.keys()) + 1) + "}\n\\hline\n"
    
    # Dodanie nagłówków do tabeli
    latex_table += " & ".join(headers) + " \\\\\n\\hline\n"
    
    # Dla każdej ilości wierzchołków
    for vertices in data[list(data.keys())[0]]:
        # Przekształcenie tablicy numpy w listę lub wyodrębnienie pojedynczych wartości
        if isinstance(vertices, np.ndarray):
            vertices_str = ", ".join(map(str, vertices.tolist()))
        else:
            vertices_str = str(vertices)
        row = vertices_str
        # Dla każdej gęstości
        for density in data:
            # Znalezienie czasu dla danej ilości wierzchołków i gęstości
            time = None
            for item in data[density]:
                if np.array_equal(item[0], vertices):
                    time = item[1]
                    break
            # Formatowanie czasu do dwóch miejsc po przecinku
            time_str = f"{time:.2f}" if time is not None else "-"
            # Dodanie czasu do wiersza
            row += f" & {time_str}"
        # Dodanie wiersza do tabeli
        latex_table += row + " \\\\\n"
    
    # Zamknięcie tabeli LaTeX
    latex_table += "\\hline\n\\end{tabular}\n\\caption{Dane dla grafu " + graph.upper() + " i algorytmu " + algorithm.upper() + "}\n\\end{table}"
    
    return latex_table

# Generowanie i zapisywanie tabel LaTeX dla wszystkich grafów i algorytmów
for graph in dane:
    for algorithm in dane[graph]:
        latex_table = generate_latex_table(graph, algorithm)
        print(latex_table)

# Dla każdego grafu
for graf, algorytmy in dane.items():
    # Dla każdego algorytmu
    for algorytm, gęstości in algorytmy.items():
        # Tworzenie nowego wykresu
        plt.figure()
        plt.title(f"Algorytm {algorytm} dla grafu {graf}")
        plt.xlabel("Ilość wierzchołków")
        plt.ylabel("Czas wykonania (ms)")
        
        # Dla każdej gęstości
        for gestość, pomiary in gęstości.items():
            ilość_wierzchołków = [pomiar[0] for pomiar in pomiary]
            czas_wykonania = [pomiar[1] / 1e6 for pomiar in pomiary]
            plt.plot(ilość_wierzchołków, czas_wykonania, marker='o', label=f"Gęstość: {gestość}")

        plt.legend()
        plt.grid(True)
        plt.show()
