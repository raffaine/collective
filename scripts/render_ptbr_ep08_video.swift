import Foundation
import AVFoundation
import CoreGraphics
import CoreText
import AppKit

struct SceneSpec {
    let path: String
    let actLabel: String
    let placeLabel: String
}

let scenes: [SceneSpec] = [
    // Parte I: Cenário Epsilon — Portal Vizinho Zero-ID, Chave de Latão VLAN 40/30, Calor dos Servidores e Carapaça como Serviço
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_01.png", actLabel: "PARTE I // CENÁRIO EPSILON: PORTAL VIZINHO ZERO-ID", placeLabel: "Café de Terceiro Lugar na Calçada e Quiosque Zero-ID Sem Login"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_02.png", actLabel: "PARTE I // CENÁRIO EPSILON: CHAVE FÍSICA VLAN 40 E VLAN 30", placeLabel: "Salão de Estudos a 40 dBA e Chave Seletora de Latão (Isolamento MDM)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_03.png", actLabel: "PARTE I // CENÁRIO EPSILON: CARAPAÇA COMO SERVIÇO E CALOR", placeLabel: "Recuperação de 80% do Calor Digital e Ato Cooperativo (Bancos BACEN)"),

    // Parte II: Cenário Iota — Ciência Cidadã Pré-Registrada, Deriva de até 50 bp, Paridade de Resultado Nulo e Biblioteca de Alexandria
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_04.png", actLabel: "PARTE II // CENÁRIO IOTA: TRAVA METROLÓGICA DE ATÉ 50 BP", placeLabel: "Laboratório Isolado de Vibração e Cadernos Pré-Registrados (0,5% Deriva)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_05.png", actLabel: "PARTE II // CENÁRIO IOTA: ENCADERNADORA DE ALEXANDRIA", placeLabel: "Biblioteca IPFS Offline e Encadernação em Papel de Trapo de 300 Anos"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_06.png", actLabel: "PARTE II // CENÁRIO IOTA: REPLICAÇÃO E RESULTADOS NULOS", placeLabel: "Observatório de Três Vales (Paridade de Resultado Nulo e Prior Art INPI)"),

    // Parte III: Cenário Lambda — Chave-Faca de Latão Sem Gravação, Luthieria Sem CITES, Folhetos Sem Algoritmo e Pontos de Cultura
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_07.png", actLabel: "PARTE III // CENÁRIO LAMBDA: CHAVE-FACA SEM GRAVAÇÃO", placeLabel: "Salão Acústico de Choro e Chave-Faca de Latão (Circuito de Mic Aberto)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_08.png", actLabel: "PARTE III // CENÁRIO LAMBDA: LUTHIERIA SEM MADEIRAS CITES", placeLabel: "Tampo de Violão Curado a até 8% de Umidade e Sinos de Bronze-Silício"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_09.png", actLabel: "PARTE III // CENÁRIO LAMBDA: FOLHETOS ZERO-ALGORITMO", placeLabel: "Prensa Tipográfica e Pigmentos Naturais (Ponto de Cultura Lei 13.018)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_10.png", actLabel: "PARTE III // CENÁRIO LAMBDA: LANTERNA PSICOACÚSTICA", placeLabel: "Palco no Bosque e Lanterna Âmbar de Decibéis (Prova ABNT NBR 10151)"),

    // Parte IV: Cenário Pi — Malha Policêntrica 3D, Enxames de Mutirão Autodissolúveis, Tratados Ricardianos FROST e MeshGrowthBeacons
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_11.png", actLabel: "PARTE IV // CENÁRIO PI: MALHA POLICÊNTRICA 3D", placeLabel: "Mapa em Relevo da Bacia e Rotunda Bicameral (Vilas e Guildas de Ofício)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_12.png", actLabel: "PARTE IV // CENÁRIO PI: MUTIRÕES AUTODISSOLÚVEIS", placeLabel: "Cordas Enroladas e Bancada Concluída (Autodissolução no Recibo Final)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_13.png", actLabel: "PARTE IV // CENÁRIO PI: TRATADOS RICARDIANOS DE 4 ESTÁGIOS", placeLabel: "Ponte de Pedra Entre Vales e Vilas-Irmãs (Treaty_CID e Limiar FROST)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_14.png", actLabel: "PARTE IV // CENÁRIO PI: CANAIS BCC E FAROL DE CRESCIMENTO", placeLabel: "Cais de Troca no Canal e Barcaça Solar (Farol MeshGrowthBeacon)"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_15.png", actLabel: "PARTE IV // SÍNTESE DA ONDA E: BACIA FEDERADA VIVA", placeLabel: "Vale Federado ao Crepúsculo e Vilas de Céu Escuro (Sem Burocracia)")
]

let width = 1920
let height = 1080
let fps: Int32 = 8
let crossfadeSec: Double = 4.0
let colorSpace = CGColorSpaceCreateDeviceRGB()
let bitmapInfo = CGImageAlphaInfo.premultipliedFirst.rawValue | CGBitmapInfo.byteOrder32Little.rawValue

func loadNormalizedCGImage(_ path: String) -> CGImage {
    guard let nsImg = NSImage(contentsOfFile: path),
          let srcCG = nsImg.cgImage(forProposedRect: nil, context: nil, hints: nil) else {
        fatalError("Failed to load image at \(path)")
    }
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create normalize CGContext")
    }
    ctx.interpolationQuality = .high
    let scaleX = CGFloat(width) / CGFloat(srcCG.width)
    let scaleY = CGFloat(height) / CGFloat(srcCG.height)
    let scale = max(scaleX, scaleY)
    let drawW = CGFloat(srcCG.width) * scale
    let drawH = CGFloat(srcCG.height) * scale
    let dx = (CGFloat(width) - drawW) * 0.5
    let dy = (CGFloat(height) - drawH) * 0.5
    ctx.draw(srcCG, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    return ctx.makeImage()!
}

func makeHudCGImage(actLabel: String, placeLabel: String) -> CGImage {
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create HUD CGContext")
    }

    let boxRect = CGRect(x: 48, y: 42, width: 920, height: 86)
    let boxPath = CGPath(roundedRect: boxRect, cornerWidth: 8, cornerHeight: 8, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.64))
    ctx.addPath(boxPath)
    ctx.fillPath()

    ctx.setStrokeColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.45))
    ctx.setLineWidth(1.5)
    ctx.addPath(boxPath)
    ctx.strokePath()

    let pillRect = CGRect(x: 48, y: height - 66, width: 780, height: 34)
    let pillPath = CGPath(roundedRect: pillRect, cornerWidth: 6, cornerHeight: 6, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.58))
    ctx.addPath(pillPath)
    ctx.fillPath()

    func drawText(_ text: String, font: CTFont, color: CGColor, x: CGFloat, y: CGFloat) {
        let attrs: [NSAttributedString.Key: Any] = [
            NSAttributedString.Key(kCTFontAttributeName as String): font,
            NSAttributedString.Key(kCTForegroundColorAttributeName as String): color
        ]
        let attrStr = NSAttributedString(string: text, attributes: attrs)
        let line = CTLineCreateWithAttributedString(attrStr)
        ctx.textPosition = CGPoint(x: x, y: y)
        CTLineDraw(line, ctx)
    }

    let fontTop = CTFontCreateWithName("Courier-Bold" as CFString, 15, nil)
    let fontAct = CTFontCreateWithName("Courier-Bold" as CFString, 15, nil)
    let fontPlace = CTFontCreateWithName("Helvetica-Bold" as CFString, 20, nil)

    let gold = CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.95)
    let green = CGColor(red: 0.48, green: 0.82, blue: 0.42, alpha: 0.95)
    let white = CGColor(red: 0.95, green: 0.96, blue: 0.98, alpha: 0.96)

    drawText("RÁDIO LAMBDA BRASIL // EP. 08: ONDA E (EPSILON, IOTA, LAMBDA E PI)", font: fontTop, color: gold, x: 64, y: CGFloat(height - 54))
    drawText(actLabel, font: fontAct, color: green, x: 68, y: 96)
    drawText(placeLabel, font: fontPlace, color: white, x: 68, y: 62)

    return ctx.makeImage()!
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    do {
        let audioPath = "doc/podcast/pt-br/episodes/08.episode.m4a"
        let finalOutPath = "doc/podcast/pt-br/episodes/08.episode_youtube.mp4"
        let tempVideoPath = "/tmp/ptbr_ep08_silent_video.mp4"

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        try? FileManager.default.removeItem(atPath: finalOutPath)

        let audioURL = URL(fileURLWithPath: audioPath)
        let audioAsset = AVURLAsset(url: audioURL)
        let durationCM = try await audioAsset.load(.duration)
        let durationSec = CMTimeGetSeconds(durationCM)
        print(String(format: "Audio duration: %.2f seconds (%.2f minutes)", durationSec, durationSec / 60.0))

        let cgScenes = scenes.map { loadNormalizedCGImage($0.path) }
        let hudScenes = scenes.map { makeHudCGImage(actLabel: $0.actLabel, placeLabel: $0.placeLabel) }
        let sceneDuration = durationSec / Double(scenes.count)

        for (i, s) in scenes.enumerated() {
            let startSec = Int(round(Double(i) * sceneDuration))
            let mm = startSec / 60
            let ss = startSec % 60
            print(String(format: "CHAPTER %02d:%02d — %@ (%@)", mm, ss, s.placeLabel, s.actLabel))
        }

        let writer = try AVAssetWriter(outputURL: URL(fileURLWithPath: tempVideoPath), fileType: .mp4)
        let videoSettings: [String: Any] = [
            AVVideoCodecKey: AVVideoCodecType.h264,
            AVVideoWidthKey: width,
            AVVideoHeightKey: height,
            AVVideoCompressionPropertiesKey: [
                AVVideoAverageBitRateKey: 1_800_000,
                AVVideoMaxKeyFrameIntervalKey: 16,
                AVVideoProfileLevelKey: AVVideoProfileLevelH264HighAutoLevel
            ]
        ]
        let writerInput = AVAssetWriterInput(mediaType: .video, outputSettings: videoSettings)
        writerInput.expectsMediaDataInRealTime = false

        let pixelBufferAttrs: [String: Any] = [
            kCVPixelBufferPixelFormatTypeKey as String: Int(kCVPixelFormatType_32BGRA),
            kCVPixelBufferWidthKey as String: width,
            kCVPixelBufferHeightKey as String: height,
            kCVPixelBufferCGImageCompatibilityKey as String: true,
            kCVPixelBufferCGBitmapContextCompatibilityKey as String: true
        ]
        let adaptor = AVAssetWriterInputPixelBufferAdaptor(
            assetWriterInput: writerInput,
            sourcePixelBufferAttributes: pixelBufferAttrs
        )

        writer.add(writerInput)
        writer.startWriting()
        writer.startSession(atSourceTime: .zero)

        let totalFrames = Int(ceil(durationSec * Double(fps)))
        print("Rendering \(totalFrames) frames at \(fps) fps across \(scenes.count) scenes...")

        func drawSceneKenBurns(_ ctx: CGContext, img: CGImage, index: Int, progress: Double, alpha: CGFloat) {
            let p = CGFloat(max(0.0, min(1.0, progress)))
            let zoomIn = (index % 2 == 0)
            let scale: CGFloat = zoomIn ? (1.02 + 0.08 * p) : (1.10 - 0.08 * p)
            let panDir: CGFloat = (index % 3 == 0) ? 1.0 : ((index % 3 == 1) ? -1.0 : 0.4)
            let maxPanX = CGFloat(width) * 0.025
            let maxPanY = CGFloat(height) * 0.015
            let offsetX = panDir * maxPanX * (p - 0.5)
            let offsetY = -panDir * maxPanY * (p - 0.5)

            let drawW = CGFloat(width) * scale
            let drawH = CGFloat(height) * scale
            let x = (CGFloat(width) - drawW) * 0.5 + offsetX
            let y = (CGFloat(height) - drawH) * 0.5 + offsetY

            ctx.saveGState()
            if alpha < 0.999 {
                ctx.setAlpha(alpha)
            }
            ctx.interpolationQuality = .low
            ctx.draw(img, in: CGRect(x: x, y: y, width: drawW, height: drawH))
            ctx.restoreGState()
        }

        for frameIdx in 0..<totalFrames {
            while !writerInput.isReadyForMoreMediaData {
                usleep(2000)
            }

            autoreleasepool {
                let t = Double(frameIdx) / Double(fps)
                let rawIdx = Int(t / sceneDuration)
                let curIdx = min(max(0, rawIdx), scenes.count - 1)
                let sceneLocalT = t - Double(curIdx) * sceneDuration
                let prog = sceneLocalT / sceneDuration

                var pixelBufferOut: CVPixelBuffer?
                CVPixelBufferPoolCreatePixelBuffer(nil, adaptor.pixelBufferPool!, &pixelBufferOut)
                guard let pixelBuffer = pixelBufferOut else { return }

                CVPixelBufferLockBaseAddress(pixelBuffer, [])
                let baseAddr = CVPixelBufferGetBaseAddress(pixelBuffer)!
                let bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer)

                let ctx = CGContext(
                    data: baseAddr,
                    width: width,
                    height: height,
                    bitsPerComponent: 8,
                    bytesPerRow: bytesPerRow,
                    space: colorSpace,
                    bitmapInfo: bitmapInfo
                )!

                drawSceneKenBurns(ctx, img: cgScenes[curIdx], index: curIdx, progress: prog, alpha: 1.0)

                if curIdx + 1 < scenes.count && (sceneDuration - sceneLocalT) < crossfadeSec {
                    let fadeAlpha = CGFloat((crossfadeSec - (sceneDuration - sceneLocalT)) / crossfadeSec)
                    drawSceneKenBurns(ctx, img: cgScenes[curIdx + 1], index: curIdx + 1, progress: 0.0, alpha: fadeAlpha)
                }

                ctx.interpolationQuality = .none
                ctx.draw(hudScenes[curIdx], in: CGRect(x: 0, y: 0, width: width, height: height))

                let totalProg = CGFloat(t / durationSec)
                ctx.setFillColor(CGColor(red: 1.0, green: 1.0, blue: 1.0, alpha: 0.18))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width), height: 6))
                ctx.setFillColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.92))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width) * totalProg, height: 6))

                CVPixelBufferUnlockBaseAddress(pixelBuffer, [])

                let pts = CMTime(value: CMTimeValue(frameIdx), timescale: fps)
                adaptor.append(pixelBuffer, withPresentationTime: pts)
            }

            if frameIdx % 1200 == 0 && frameIdx > 0 {
                let pct = Int((Double(frameIdx) / Double(totalFrames)) * 100.0)
                print("Progress: \(pct)% (\(frameIdx)/\(totalFrames) frames)")
            }
        }

        writerInput.markAsFinished()
        await writer.finishWriting()
        print("Silent video track rendered. Now muxing audio losslessly...")

        let composition = AVMutableComposition()
        let videoAsset = AVURLAsset(url: URL(fileURLWithPath: tempVideoPath))

        let compVideoTrack = composition.addMutableTrack(withMediaType: .video, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcVideoTrack = try await videoAsset.loadTracks(withMediaType: .video).first!
        try compVideoTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcVideoTrack, at: .zero)

        let compAudioTrack = composition.addMutableTrack(withMediaType: .audio, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcAudioTrack = try await audioAsset.loadTracks(withMediaType: .audio).first!
        try compAudioTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcAudioTrack, at: .zero)

        let exporter = AVAssetExportSession(asset: composition, presetName: AVAssetExportPresetPassthrough)!
        try await exporter.export(to: URL(fileURLWithPath: finalOutPath), as: .mp4)

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        print("SUCCESS: Created PT-BR Episode 08 YouTube video at \(finalOutPath)")
        semaphore.signal()
    } catch {
        print("ERROR: \(error)")
        exit(1)
    }
}

semaphore.wait()
